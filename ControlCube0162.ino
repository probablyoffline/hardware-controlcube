/* 
Control Cube v0.1.6.2 [untested]
*/

/*~~~~~~~~~~~VARIABLES~~~~~~~~~~~*/
#include <SPI.h>

#define blueS7Pin 14
#define redS7Pin 15

#define ledRed 9
#define ledBlue 8

#define lowHorn 6
#define highHorn 7

#define blueButPin 5
#define redButPin 4

int blueButtonState;
int lastBlueButtonState = LOW;
int redButtonState;
int lastRedButtonState = LOW;
int hornLoopCnt = 1;
char blueString[10];
char redString[10];
char menuString1[10];
char menuString2[10];
char delayString[10];
unsigned int winScore = 1; /// Default score. Changes depending on the options you choose for scoring. Points or Time.
int startDelay = 0;
int delayTime;
int delayTime2;
unsigned long firstBluePress;
unsigned long firstRedPress;
unsigned long firstTimer;
boolean isBlueCounting;
boolean isRedCounting;
boolean isScoreUp = false;
boolean isScoreDown = false;
long countRate = 500;
int newGame = 1;
unsigned int blueScore = 0;
unsigned int redScore = 0;
long preBlueMillis;
long preRedMillis;
int resetCount;
int menuOption;
int bootMenu;
int holdCount = 5;
int accelCountSpd = 0;

void setup() 
{
  pinMode(ledRed, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(lowHorn, OUTPUT);
  pinMode(highHorn, OUTPUT);
  digitalWrite(lowHorn, LOW);
  digitalWrite(highHorn, LOW);
  pinMode(blueS7Pin, OUTPUT);
  digitalWrite(blueS7Pin, HIGH); 
  pinMode(redS7Pin, OUTPUT);
  digitalWrite(redS7Pin, HIGH);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV64);
  setBrightnessBlue(255);
  setBrightnessRed(255);
  
/*~~~~~~~~~~~BOOT SEQUENCE~~~~~~~~~~~*/
  sendStringBlue("boot");
  sendStringRed("boot");
  
	delay(1500);
	bootMenu = 1;
	menuOption = 1;
	lastBlueButtonState = HIGH;
	lastRedButtonState = HIGH;
	
	//while(bootMenu == 1)
	//{	

		while(menuOption == 1)
		{
			blueButtonState = digitalRead(blueButPin);
			redButtonState = digitalRead(redButPin);
			sendStringRed("SCOr");
			sendStringBlue("CLOc");
			
			if (blueButtonState == HIGH)
			{
				winScore = 600; /// Starting score for Time in seconds.
				//lastBlueButtonState = LOW;
				//lastRedButtonState = LOW;
				firstRedPress = 0;
				menuOption == 2;
			}
			/*
			if (redButtonState == LOW && blueButtonState == LOW)
			{
				isScoreDown = false;
				isScoreUp = false;
			}
			
			if (blueButtonState == HIGH && redButtonState == LOW && isScoreUp == false)
			{
				firstBluePress = millis();
				isScoreUp = true;
			}
			
			
			if (blueButtonState == HIGH && (millis() - firstBluePress >= 80) && isScoreUp == true)///blue button for clock
			{
				menuOption == 10;
				winScore = 600; /// Starting score for Time in seconds.
			}
			
			if (blueButtonState == LOW && redButtonState == HIGH && isScoreDown == false)
			{
				firstRedPress = millis();
				isScoreDown = true;
			}
		
			if (redButtonState == HIGH && (millis() - firstRedPress >= 80) && isScoreDown == true) ///red button for score
			{
				menuOption == 11;
				winScore = 1200; /// Starting score for Points. (2 points per second)
			}
			*/
			lastBlueButtonState = blueButtonState;
			lastRedButtonState = redButtonState;
		}
		
		clearDisplaySPI();
		resetCursor();
		
		while(menuOption == 2)///time (blue button)  options
		{
			blueButtonState = digitalRead(blueButPin);
			redButtonState = digitalRead(redButPin);
			sprintf(menuString1, "%4d", winScore);
			sendStringRed(menuString1);
			sendStringBlue("2222");
		}
		/*
		while(menuOption == 2)///time (blue button)  options
		{
			blueButtonState = digitalRead(blueButPin);
			redButtonState = digitalRead(redButPin);
			sprintf(menuString1, "%4d", winScore);
			sendStringRed(menuString1);
			sendStringBlue("CLOc");
			if ((blueButtonState == HIGH && redButtonState == HIGH && lastBlueButtonState == LOW) || (blueButtonState == HIGH && redButtonState == HIGH && lastRedButtonState == LOW))
			{
				menuOption = 2;
				lastBlueButtonState = LOW;
				lastRedButtonState = LOW;
				firstBluePress = 0;
				firstRedPress = 0;
			}

			if (redButtonState == LOW && blueButtonState == LOW)
			{
				isScoreDown = false;
				isScoreUp = false;
				accelCountSpd = 0;
			}

			if (blueButtonState == HIGH && redButtonState == LOW && isScoreUp == false)
			{
				firstBluePress = millis();
				isScoreUp = true;
				accelCountSpd = 0;
			}
			
			
			if (blueButtonState == HIGH && (millis() - firstBluePress >= 80) && isScoreUp == true)
			{
				if (accelCountSpd >= 40)
				{
					winScore = winScore + 1;
						if (winScore > 9999)
						{
							winScore = 1;
						}
						delay(35);
				}
				
				else
				{
					if (accelCountSpd >= 9)
					{
						winScore = winScore + 10;
						if (winScore > 9999)
						{
							winScore = 1;
						}
						delay(100);
					}
					
					else
					{
						winScore = winScore + 60;
						if (winScore > 9999)
						{
							winScore = 1;
						}
						delay(200);
					}
				}
				
				accelCountSpd++;
				sprintf(menuString1, "%4d", winScore);
				sendStringRed(menuString1);								
			}

			if (blueButtonState == LOW && redButtonState == HIGH && isScoreDown == false)
			{
				firstRedPress = millis();
				isScoreDown = true;
				accelCountSpd = 0;
			}

			if (redButtonState == HIGH && (millis() - firstRedPress >= 80) && isScoreDown == true)
			{
				if (accelCountSpd >= 40)
				{
					winScore = winScore - 1;
						if (winScore < 1)
						{
							winScore = 9999;
						}
						delay(35);
				}
				
				else
				{
					if (accelCountSpd >= 9)
					{
						winScore = winScore - 10;
						if (winScore < 1)
						{
							winScore = 9999;
						}
						delay(100);
					}
					
					else
					{
						winScore = winScore - 60;
						if (winScore < 1)
						{
							winScore = 9999;
						}
						delay(200);
					}
				}
				
				accelCountSpd++;
				sprintf(menuString1, "%4d", winScore);
				sendStringBlue(menuString1);
			}

			lastBlueButtonState = blueButtonState;
			lastRedButtonState = redButtonState;
			
			clearDisplaySPI();
			resetCursor();
		}
		
		
		while(menuOption == 11) ///score (red button) options
		{
			blueButtonState = digitalRead(blueButPin);
			redButtonState = digitalRead(redButPin);
			sprintf(menuString1, "%4d", winScore);
			sendStringBlue(menuString1);
			sendStringRed("SCOr");
			if ((blueButtonState == HIGH && redButtonState == HIGH && lastBlueButtonState == LOW) || (blueButtonState == HIGH && redButtonState == HIGH && lastRedButtonState == LOW))
			{
				menuOption = 2;
				lastBlueButtonState = LOW;
				lastRedButtonState = LOW;
				firstBluePress = 0;
				firstRedPress = 0;
			}

			if (redButtonState == LOW && blueButtonState == LOW)
			{
				isScoreDown = false;
				isScoreUp = false;
				accelCountSpd = 0;
			}

			if (blueButtonState == HIGH && redButtonState == LOW && isScoreUp == false)
			{
				firstBluePress = millis();
				isScoreUp = true;
				accelCountSpd = 0;
			}

			if (blueButtonState == HIGH && (millis() - firstBluePress >= 80) && isScoreUp == true)
			{
				if (accelCountSpd >= 40)
				{
					winScore = winScore + 10;
						if (winScore > 9999)
						{
							winScore = 10;
						}
						delay(35);
				}
				
				else
				{
					if (accelCountSpd >= 9)
					{
						winScore = winScore + 10;
						if (winScore > 9999)
						{
							winScore = 10;
						}
						delay(100);
					}
					
					else
					{
						winScore = winScore + 10;
						if (winScore > 9999)
						{
							winScore = 10;
						}
						delay(200);
					}
				}
				
				accelCountSpd++;
				sprintf(menuString1, "%4d", winScore);
				sendStringBlue(menuString1);								
			}

			if (blueButtonState == LOW && redButtonState == HIGH && isScoreDown == false)
			{
				firstRedPress = millis();
				isScoreDown = true;
				accelCountSpd = 0;
			}

			if (redButtonState == HIGH && (millis() - firstRedPress >= 80) && isScoreDown == true)
			{
				if (accelCountSpd >= 40)
				{
					winScore = winScore - 10;
						if (winScore < 1)
						{
							winScore = 9999;
						}
						delay(35);
				}
				
				else
				{
					if (accelCountSpd >= 9)
					{
						winScore = winScore - 10;
						if (winScore < 1)
						{
							winScore = 9999;
						}
						delay(100);
					}
					
					else
					{
						winScore = winScore - 10;
						if (winScore < 1)
						{
							winScore = 9999;
						}
						delay(200);
					}
				}
				
				accelCountSpd++;
				sprintf(menuString1, "%4d", winScore);
				sendStringBlue(menuString1);
			}

			lastBlueButtonState = blueButtonState;
			lastRedButtonState = redButtonState;
			
			clearDisplaySPI();
			resetCursor();
		}
	//}
	
	while(menuOption == 4)
	{
		blueButtonState = digitalRead(blueButPin);
		redButtonState = digitalRead(redButPin);
		sprintf(menuString2, "%4d", startDelay);
		setDecimalsBlueSPI(0b010000);
		sendStringRed("dLAY");
		
		sendStringBlue(menuString2);
		if ((blueButtonState == HIGH && redButtonState == HIGH && lastBlueButtonState == LOW) || (blueButtonState == HIGH && redButtonState == HIGH && lastRedButtonState == LOW))
		{
			menuOption = 3;
			lastBlueButtonState = LOW;
			lastRedButtonState = LOW;
		}
		
		if (redButtonState == LOW && blueButtonState == LOW)
		{
			isScoreDown = false;
			isScoreUp = false;
		}

		if (blueButtonState == HIGH && redButtonState == LOW && isScoreUp == false)
		{
			firstBluePress = millis();
			isScoreUp = true;
			accelCountSpd = 0;
		}

		if (blueButtonState == HIGH && (millis() - firstBluePress >= 80) && isScoreUp == true)
		{
			if (accelCountSpd >= 9)
			{
				startDelay = startDelay + 100;
				if (startDelay > 6000)
				{
					startDelay = 6000;
					
				}
				delay(100);
			}
			
			else
			{
				startDelay = startDelay + 100;
				if (startDelay > 6000)
				{
					startDelay = 6000;
					
				}
				delay(200);
			}
			
			accelCountSpd++;
			sprintf(menuString2, "%4d", startDelay);
			setDecimalsBlueSPI(0b010000);
			sendStringBlue(menuString2);
		}

		if (blueButtonState == LOW && redButtonState == HIGH && isScoreDown == false)
		{
			firstRedPress = millis();
			isScoreDown = true;
			accelCountSpd = 0;
		}

		if (redButtonState == HIGH && (millis() - firstRedPress >= 80) && isScoreDown == true)
		{
			if(accelCountSpd >= 9)
			{
				startDelay = startDelay - 100;
				if (startDelay < 0)
				{
					startDelay = 0;
				}
				delay(100);
			}
			
			else
			{
				startDelay = startDelay - 100;
				if (startDelay < 0)
				{
					startDelay = 0;
				}
				delay(200);
			}
			
			
			
			accelCountSpd++;
			sprintf(menuString2, "%4d", startDelay);
			setDecimalsBlueSPI(0b010000);
			sendStringBlue(menuString2);
		}

		lastBlueButtonState = blueButtonState;
		lastRedButtonState = redButtonState;
		
	}
	

while(menuOption == 5)
	{
		blueButtonState = digitalRead(blueButPin);
		redButtonState = digitalRead(redButPin);
		sprintf(menuString2, "%4d", holdCount);
		sendStringRed("HoLd");
		setDecimalsBlueSPI(0b000000);
		
		sendStringBlue(menuString2);
		if ((blueButtonState == HIGH && redButtonState == HIGH && lastBlueButtonState == LOW) || (blueButtonState == HIGH && redButtonState == HIGH && lastRedButtonState == LOW))
		{
			bootMenu = 0;
			menuOption = 0;
			lastBlueButtonState = LOW;
			lastRedButtonState = LOW;
		}
		
		if (redButtonState == LOW && blueButtonState == LOW)
		{
			isScoreDown = false;
			isScoreUp = false;
		}

		if (blueButtonState == HIGH && redButtonState == LOW && isScoreUp == false)
		{
			firstBluePress = millis();
			isScoreUp = true;
		}

		if (blueButtonState == HIGH && (millis() - firstBluePress >= 80) && isScoreUp == true)
		{
			holdCount = 5;
			sprintf(menuString2, "%4d", holdCount);
			sendStringBlue(menuString2);
		}

		if (blueButtonState == LOW && redButtonState == HIGH && isScoreDown == false)
		{
			firstRedPress = millis();
			isScoreDown = true;
		}

		if (redButtonState == HIGH && (millis() - firstRedPress >= 80) && isScoreDown == true)
		{
			holdCount = 1;
			sprintf(menuString2, "%4d", holdCount);
			sendStringBlue(menuString2);
		}
		
		lastBlueButtonState = blueButtonState;
		lastRedButtonState = redButtonState;
	}
	
	*/
} 

void loop()
{
	if (newGame == 1)
	{
		clearDisplaySPI();
		resetCursor();
		sendStringRed("StAr");
		sendStringBlue("t   ");
		delay(500);
		sendStringRed(" StA");
		sendStringBlue("rt  ");
		delay(500);
		sendStringRed("  St");
		sendStringBlue("Art ");
		delay(500);
		sendStringRed("   S");
		sendStringBlue("tArt");
		delay(3000);
		//clearDisplaySPI();
		delayTime = (startDelay/100)*60;
		while(delayTime > 0)
		{
			delayTime2 = ((delayTime/60)*100) + (delayTime%60);
			sprintf(delayString, "%4d", delayTime2);
			sendStringRed(delayString);
			setDecimalsSPI(0b010000);
			sendStringBlue(delayString);
			setDecimalsSPI(0b010000);
			delayTime--;
			delay(1000);
		}
		
		digitalWrite(lowHorn, HIGH);
		delay(250);
		digitalWrite(lowHorn, LOW);
		delay(750);
		digitalWrite(lowHorn, HIGH);
		delay(250);
		digitalWrite(lowHorn, LOW);
		delay(750);
		digitalWrite(lowHorn, HIGH);
		delay(250);
		digitalWrite(lowHorn, LOW);
		delay(750);
		digitalWrite(lowHorn, HIGH);
		digitalWrite(highHorn, HIGH);
		clearDisplaySPI();
		sendStringRed("rdY-");
		sendStringBlue("rdY-");
		delay(3000);
		digitalWrite(highHorn, LOW);
		digitalWrite(lowHorn, LOW);
		lastBlueButtonState = LOW;
		lastRedButtonState = LOW;
		firstBluePress = 0;
		firstRedPress = 0;
		firstTimer = 0;
		newGame = 0;
	}

/*~~~~~~~~~~~BLUE STATE CHANGE~~~~~~~~~~~*/
  blueButtonState = digitalRead(blueButPin);
  redButtonState = digitalRead(redButPin);
  
	if (blueButtonState == HIGH && lastBlueButtonState == LOW && isBlueCounting == false) 
	{
		firstBluePress = millis();
		lastBlueButtonState = HIGH;
		sendStringBlue("HoLd");
	}
  
  if(holdCount == 1)
  {
	 if (blueButtonState == HIGH && millis() - firstBluePress >= 1000 && isBlueCounting == false)
	{
		sendStringBlue("ctrL");
		isBlueCounting = true;
		isRedCounting = false;
		firstBluePress = 0;
		lastBlueButtonState = LOW;
		digitalWrite(ledRed, LOW);
		digitalWrite(ledBlue, HIGH);
		digitalWrite(lowHorn, HIGH);
		delay(250);
		digitalWrite(lowHorn, LOW);
	} 
  }
  
  if(holdCount == 5)
  {
	if (blueButtonState == HIGH && (millis() - firstBluePress >= 1000) && (millis() - firstBluePress <= 1050) && isBlueCounting == false)
	{
		sendStringBlue("2oLd");
	}
  
	if (blueButtonState == HIGH && (millis() - firstBluePress >= 2000) && (millis() - firstBluePress <= 2050) && isBlueCounting == false)
	{
		sendStringBlue("H3Ld");
	}
  
	if (blueButtonState == HIGH && (millis() - firstBluePress >= 3000) && (millis() - firstBluePress <= 3050) && isBlueCounting == false)
	{
		sendStringBlue("Ho4d");
	}
	
	if (blueButtonState == HIGH && millis() - firstBluePress >= 4000 && isBlueCounting == false)
	{
		sendStringBlue("HoL5");
	}
	
	if (blueButtonState == HIGH && millis() - firstBluePress >= 5000 && isBlueCounting == false)
	{
		sendStringBlue("ctrL");
		isBlueCounting = true;
		isRedCounting = false;
		firstBluePress = 0;
		lastBlueButtonState = LOW;
		digitalWrite(ledRed, LOW);
		digitalWrite(ledBlue, HIGH);
		digitalWrite(ledBlue, HIGH);
		digitalWrite(lowHorn, HIGH);
		delay(250);
		digitalWrite(lowHorn, LOW);
	}
  }
  
	if (blueButtonState == LOW && lastBlueButtonState == HIGH && isBlueCounting == false) 
	{
		lastBlueButtonState = LOW;
		sendStringBlue(blueString); 
		firstBluePress = 0;
	}

	  
/*~~~~~~~~~~~RED STATE CHANGE~~~~~~~~~~~*/
	if (redButtonState == HIGH && lastRedButtonState == LOW && isRedCounting == false) 
	{
		firstRedPress = millis();
		lastRedButtonState = HIGH;
		sendStringRed("HoLd");
	}

	if(holdCount == 1)
	{
		if (redButtonState == HIGH && millis() - firstRedPress >= 1000 && isRedCounting == false)
		{
			sendStringRed("ctrL");
			isRedCounting = true;
			isBlueCounting = false;
			firstRedPress = 0;
			lastRedButtonState = LOW;
			digitalWrite(ledRed, HIGH);
			digitalWrite(ledBlue, LOW);
			digitalWrite(lowHorn, HIGH);
			delay(250);
			digitalWrite(lowHorn, LOW);
		}
	}
		
	
	if(holdCount == 5)
	{
		if (redButtonState == HIGH && (millis() - firstRedPress >= 1000) && (millis() - firstRedPress <= 1050) && isRedCounting == false)
		{
			sendStringRed("2oLd");
		}
	  
		if (redButtonState == HIGH && (millis() - firstRedPress >= 2000) && (millis() - firstRedPress <= 2050) && isRedCounting == false)
		{
			sendStringRed("H3Ld");
		}
	  
		if (redButtonState == HIGH && (millis() - firstRedPress >= 3000) && (millis() - firstRedPress <= 3050) && isRedCounting == false)
		{
			sendStringRed("Ho4d");
		}
		
		if (redButtonState == HIGH && millis() - firstRedPress >= 4000 && isRedCounting == false)
		{
			sendStringRed("HoL5");
		}

		if (redButtonState == HIGH && millis() - firstRedPress >= 5000 && isRedCounting == false)
		{
			sendStringRed("ctrL");
			isRedCounting = true;
			isBlueCounting = false;
			firstRedPress = 0;
			lastRedButtonState = LOW;
			digitalWrite(ledRed, HIGH);
			digitalWrite(ledBlue, LOW);
			digitalWrite(lowHorn, HIGH);
			delay(250);
			digitalWrite(lowHorn, LOW);
		}
	}
	
	if (redButtonState == LOW && lastRedButtonState == HIGH && isRedCounting == false) 
	{
		lastRedButtonState = LOW;
		sendStringRed(redString); 
		firstRedPress = 0;
	}

  
	unsigned long curBlueMillis = millis();

/*~~~~~~~~~~~BLUE COUNTING~~~~~~~~~~~*/	
	if (isBlueCounting == true && curBlueMillis - preBlueMillis > countRate)
	{
		blueScore = blueScore + 1;
		sprintf(blueString, "%4d", blueScore);
		sendStringBlue(blueString);
		preBlueMillis = curBlueMillis;
		
		if (blueScore == winScore)
		{
			sprintf(blueString, "%4d", blueScore);
			sprintf(redString, "%4d", redScore);
			digitalWrite(lowHorn, LOW);
			digitalWrite(highHorn, HIGH);
			delay(400);
			
			digitalWrite(lowHorn, HIGH);
			digitalWrite(highHorn, LOW);
			delay(400);
			
			digitalWrite(lowHorn, LOW);
			digitalWrite(highHorn, HIGH);
			delay(400);
			
			digitalWrite(lowHorn, HIGH);
			digitalWrite(highHorn, LOW);
			delay(400);
			
			digitalWrite(lowHorn, LOW);
			digitalWrite(highHorn, HIGH);
			delay(400);
			
			digitalWrite(lowHorn, HIGH);
			digitalWrite(highHorn, LOW);
			delay(400);
			
			digitalWrite(lowHorn, HIGH);
			digitalWrite(highHorn, HIGH);
			delay(2000);
			digitalWrite(lowHorn, LOW);
			digitalWrite(highHorn, LOW);
			while(blueScore == winScore)		                                                          
			{  
				blueButtonState = digitalRead(blueButPin);
				redButtonState = digitalRead(redButPin);
				if (redButtonState == HIGH && blueButtonState == HIGH && lastBlueButtonState == HIGH && lastRedButtonState == HIGH)
				{
					resetCount++;
					if (resetCount == 4)
					{
						newGame = 1;
						firstBluePress = 0;
						firstRedPress = 0;
						isBlueCounting = false;
						isRedCounting = false;
						preBlueMillis = 0;
						preRedMillis = 0;
						blueScore = 0;
						redScore = 0;
						sprintf(blueString, "%4d", blueScore);
						sprintf(redString, "%4d", redScore);
						lastBlueButtonState = LOW;
						lastRedButtonState = LOW;
						digitalWrite(lowHorn, LOW);
						digitalWrite(highHorn, LOW);  
						digitalWrite(ledRed, LOW);
						digitalWrite(ledBlue, LOW);
						resetCount = 0;
					}
				}
				
				lastBlueButtonState = blueButtonState;
				lastRedButtonState = redButtonState;
				digitalWrite(ledRed, LOW);
				digitalWrite(ledBlue, HIGH);
				sendStringBlue("End-");
				delay(100);
				clearDisplaySPI();
				delay(100);
				sendStringBlue("End-");
				delay(100);
				clearDisplaySPI();
				delay(100);
				sendStringBlue("End-");
				delay(100);
				clearDisplaySPI();
				delay(100);
				digitalWrite(ledRed, LOW);
				digitalWrite(ledBlue, LOW);
				sendStringBlue(blueString);
				sendStringRed(redString);
				delay(100);
				clearDisplaySPI();
				delay(100);
				sendStringBlue(blueString);
				sendStringRed(redString);
				delay(100);
				clearDisplaySPI();
				delay(100);
				sendStringBlue(blueString);
				sendStringRed(redString);
				delay(100);
				clearDisplaySPI();
				delay(100); 
				if (blueButtonState == LOW || redButtonState == LOW) 
				{
					lastBlueButtonState = LOW;
					lastRedButtonState = LOW;
					resetCount = 0;
				}
			}
		}
	}		
  
  unsigned long curRedMillis = millis();
  
/*~~~~~~~~~~~RED COUNTING~~~~~~~~~~~*/  
	if (isRedCounting == true && curRedMillis - preRedMillis > countRate)
	{
		redScore = redScore + 1; 
		sprintf(redString, "%4d", redScore);
		sendStringRed(redString);
		preRedMillis = curRedMillis;

		if (redScore == winScore)
		{
			sprintf(blueString, "%4d", blueScore);
			sprintf(redString, "%4d", redScore);
			digitalWrite(lowHorn, LOW);
			digitalWrite(highHorn, HIGH);
			delay(200);
			
			digitalWrite(lowHorn, HIGH);
			digitalWrite(highHorn, LOW);
			delay(200);
			
			digitalWrite(lowHorn, LOW);
			digitalWrite(highHorn, HIGH);
			delay(200);
			
			digitalWrite(lowHorn, HIGH);
			digitalWrite(highHorn, LOW);
			delay(200);
			
			digitalWrite(lowHorn, LOW);
			digitalWrite(highHorn, HIGH);
			delay(200);
			
			digitalWrite(lowHorn, HIGH);
			digitalWrite(highHorn, LOW);
			delay(200);
			
			digitalWrite(lowHorn, HIGH);
			digitalWrite(highHorn, HIGH);
			delay(2000);
			digitalWrite(lowHorn, LOW);
			digitalWrite(highHorn, LOW);
			while(redScore == winScore) 
			{
				blueButtonState = digitalRead(blueButPin);
				redButtonState = digitalRead(redButPin);
				if (redButtonState == HIGH && blueButtonState == HIGH && lastBlueButtonState == HIGH && lastRedButtonState == HIGH)
				{
					resetCount++;
					if (resetCount == 4)
					{
						newGame = 1;
						firstBluePress = 0;
						firstRedPress = 0;
						isBlueCounting = false;
						isRedCounting = false;
						preBlueMillis = 0;
						preRedMillis = 0;
						blueScore = 0;
						redScore = 0;
						sprintf(blueString, "%4d", blueScore);
						sprintf(redString, "%4d", redScore);
						lastBlueButtonState = LOW;
						lastRedButtonState = LOW;
						digitalWrite(lowHorn, LOW);
						digitalWrite(highHorn, LOW);  
						digitalWrite(ledRed, LOW);
						digitalWrite(ledBlue, LOW);
						resetCount = 0;
					}
				}
				
				lastBlueButtonState = blueButtonState;
				lastRedButtonState = redButtonState;
				digitalWrite(ledRed, HIGH);
				digitalWrite(ledBlue, LOW);
				sendStringRed("End-");
				delay(100);
				clearDisplaySPI();
				delay(100);
				sendStringRed("End-");
				delay(100);
				clearDisplaySPI();
				delay(100);
				sendStringRed("End-");
				delay(100);
				clearDisplaySPI();
				delay(100);
				digitalWrite(ledRed, LOW);
				digitalWrite(ledBlue, LOW);
				sendStringBlue(blueString);
				sendStringRed(redString);
				delay(100);
				clearDisplaySPI();
				delay(100);
				sendStringBlue(blueString);
				sendStringRed(redString);
				delay(100);
				clearDisplaySPI();
				delay(100);
				sendStringBlue(blueString);
				sendStringRed(redString);
				delay(100);
				clearDisplaySPI();
				delay(100); 		
				if (blueButtonState == LOW || redButtonState == LOW) 
				{
					lastBlueButtonState = LOW;
					lastRedButtonState = LOW;
					resetCount = 0;
				}
			}
		}	
	}
}
 

void sendStringBlue(String toSend)
{
  digitalWrite(blueS7Pin, LOW);
  for (int i=0; i<4; i++)
  {
    SPI.transfer(toSend[i]);
  }
  digitalWrite(blueS7Pin, HIGH);
}

void sendStringRed(String toSend)
{
  digitalWrite(redS7Pin, LOW);
  for (int i=0; i<4; i++)
  {
    SPI.transfer(toSend[i]);
  }
  digitalWrite(redS7Pin, HIGH);
}

void clearDisplaySPI()
{
  digitalWrite(blueS7Pin, LOW);
  digitalWrite(redS7Pin, LOW);
  SPI.transfer(0x76);
  digitalWrite(blueS7Pin, HIGH);
  digitalWrite(redS7Pin, HIGH);
}

void clearBlue()
{
  digitalWrite(blueS7Pin, LOW);
  SPI.transfer(0x76);
  digitalWrite(blueS7Pin, HIGH);
}

void clearRed()
{
  digitalWrite(redS7Pin, LOW);
  SPI.transfer(0x76);
  digitalWrite(redS7Pin, HIGH);
}

void setBrightnessBlue(byte value)
{
  digitalWrite(blueS7Pin, LOW);
  SPI.transfer(0x7A);  // Set brightness command byte
  SPI.transfer(value);  // brightness data byte
  digitalWrite(blueS7Pin, HIGH);
}

void setBrightnessRed(byte value)
{
  digitalWrite(redS7Pin, LOW);
  SPI.transfer(0x7A);  // Set brightness command byte
  SPI.transfer(value);  // brightness data byte
  digitalWrite(redS7Pin, HIGH);
}

void resetCursor()
{
  SPI.transfer(0x7A);
  SPI.transfer(0x00);
}

void setDecimalsSPI(byte decimals)
{
  digitalWrite(redS7Pin, LOW);
  digitalWrite(blueS7Pin, LOW);
  SPI.transfer(0x77);
  SPI.transfer(decimals);
  digitalWrite(redS7Pin, HIGH);
  digitalWrite(blueS7Pin, HIGH);
}

void setDecimalsRedSPI(byte decimals)
{
  digitalWrite(redS7Pin, LOW);
  SPI.transfer(0x77);
  SPI.transfer(decimals);
  digitalWrite(redS7Pin, HIGH);
}

void setDecimalsBlueSPI(byte decimals)
{
  digitalWrite(blueS7Pin, LOW);
  SPI.transfer(0x77);
  SPI.transfer(decimals);
  digitalWrite(blueS7Pin, HIGH);
}
