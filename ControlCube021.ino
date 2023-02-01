/* 
Control Cube v0.2.1 [untested]

*****Changelog*****
-Changed all S7S display functions to utilize LCD instead
*/
#include <LiquidCrystal.h> //library for the 16x2 LCD

LiquidCrystal lcd(12, 13, 14, 15, 16, 17); //initialize the library with the numbers of the interface pins

int brightness = 150;//you can change the overall brightness by range 0 -> 255

// #include <SPI.h>

// #define blueS7Pin 14
// #define redS7Pin 15

#define ledRed 11
#define ledBlue 10

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
unsigned int winScore = 1000; // Default score, ~8 minutes. (Score/2) = total hold time seconds to win.
int startDelay = 0;
int delayTime;
int delayTime2;
int delayTime3;
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
int menuOpts;
int holdCount = 5;
int bPos;
int bootTimer = 5000;
int bTime = 0;
String boot1 = "Booting";
String boot2 = "Start";

/*
byte bar[8] = 
{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
*/

void setup() 
{
lcd.begin(16, 2);
  pinMode(ledRed, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(lowHorn, OUTPUT);
  pinMode(highHorn, OUTPUT);
  digitalWrite(lowHorn, LOW);
  digitalWrite(highHorn, LOW);
  
/*~~~~~~~~~~~BOOT SEQUENCE~~~~~~~~~~~*/

	lcd.setCursor(0, 0);
	lcd.print(boot1);
  
  while(bTime < bootTimer)
  {
	delay(500);
	bTime = bTime + 500;
	blueButtonState = digitalRead(blueButPin);
	redButtonState = digitalRead(redButPin);
	if (blueButtonState == HIGH && redButtonState == HIGH)
	  {
		menuOpts = 1;
		lastBlueButtonState = HIGH;
		lastRedButtonState = HIGH;
	  }
	lcd.clear();
	lcd.setCursor(0, 0);
	boot1 = boot1 + '.';
	lcd.print(boot1);
  }
  
  bTime = 0;
  
	  
/*~~~~~~~~~~~MENU 1~~~~~~~~~~~*/
	while(menuOpts == 1)
	{
		blueButtonState = digitalRead(blueButPin);
		redButtonState = digitalRead(redButPin);
		lcd.setCursor(0, 0);
		lcd.print("Winning Score");
		lcd.setCursor(0, 1);
		lcd.print(winScore);
		if ((blueButtonState == HIGH && redButtonState == HIGH && lastBlueButtonState == LOW) || (blueButtonState == HIGH && redButtonState == HIGH && lastRedButtonState == LOW))
		{
			menuOpts = 2;
			lastBlueButtonState = LOW;
			lastRedButtonState = LOW;
			firstBluePress = 0;
			firstRedPress = 0;
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

		if (blueButtonState == HIGH && (millis() - firstBluePress >= 1000) && isScoreUp == true)
		{
			winScore = winScore + 10;
			if (winScore > 9999)
			{
				winScore = 9999;
			}
			delay(200);
			lcd.setCursor(0, 1);
			lcd.print(winScore);							
		}

		if (blueButtonState == LOW && redButtonState == HIGH && isScoreDown == false)
		{
			firstRedPress = millis();
			isScoreDown = true;
		}

		if (redButtonState == HIGH && (millis() - firstRedPress >= 1000) && isScoreDown == true)
		{
			winScore = winScore - 10;
			if (winScore <= 10)
			{
				winScore = 10;
			}
			delay(200);
			lcd.setCursor(0, 1);
			lcd.print(winScore);
		}

		lastBlueButtonState = blueButtonState;
		lastRedButtonState = redButtonState;
	}
		
/*~~~~~~~~~~~MENU 2~~~~~~~~~~~*/		
	while(menuOpts == 2)
	{
		blueButtonState = digitalRead(blueButPin);
		redButtonState = digitalRead(redButPin);
		lcd.setCursor(0, 0);
		lcd.print("Game Delay Timer");
		lcd.setCursor(0, 1);
		lcd.print(startDelay);
		
		
		if ((blueButtonState == HIGH && redButtonState == HIGH && lastBlueButtonState == LOW) || (blueButtonState == HIGH && redButtonState == HIGH && lastRedButtonState == LOW))
		{
			menuOpts = 3;
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

		if (blueButtonState == HIGH && (millis() - firstBluePress >= 1000) && isScoreUp == true)
		{
			startDelay = startDelay + 100;
			if (startDelay > 6000)
			{
				startDelay = 6000;
				
			}
			delay(200);
			lcd.setCursor(0, 1);
			lcd.print(startDelay);
		}

		if (blueButtonState == LOW && redButtonState == HIGH && isScoreDown == false)
		{
			firstRedPress = millis();
			isScoreDown = true;
		}

		if (redButtonState == HIGH && (millis() - firstRedPress >= 1000) && isScoreDown == true)
		{
			startDelay = startDelay - 100;
			if (startDelay < 0)
			{
				startDelay = 0;
			}
			delay(200);
			lcd.setCursor(0, 1);
			lcd.print(startDelay);
		}

		lastBlueButtonState = blueButtonState;
		lastRedButtonState = redButtonState;		
	}
	
/*~~~~~~~~~~~MENU 3~~~~~~~~~~~*/
while(menuOpts == 3)
	{
		blueButtonState = digitalRead(blueButPin);
		redButtonState = digitalRead(redButPin);
		lcd.setCursor(0, 0);
		lcd.print("Hold Count");
		lcd.setCursor(0, 1);
		lcd.print(holdCount);
		
		
		
		if ((blueButtonState == HIGH && redButtonState == HIGH && lastBlueButtonState == LOW) || (blueButtonState == HIGH && redButtonState == HIGH && lastRedButtonState == LOW))
		{
			menuOpts = 0;
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

		if (blueButtonState == HIGH && (millis() - firstBluePress >= 1000) && isScoreUp == true)
		{
			holdCount = 5;
			lcd.setCursor(0, 1);
			lcd.print(holdCount);
		}

		if (blueButtonState == LOW && redButtonState == HIGH && isScoreDown == false)
		{
			firstRedPress = millis();
			isScoreDown = true;
		}

		if (redButtonState == HIGH && (millis() - firstRedPress >= 1000) && isScoreDown == true)
		{
			holdCount = 1;
			lcd.setCursor(0, 1);
			lcd.print(holdCount);
		}
		
		lastBlueButtonState = blueButtonState;
		lastRedButtonState = redButtonState;
		
	}
}

void loop()
{
	if (newGame == 1)
	{
		  while(bTime < 1100)
		  {
			delay(100);
			bTime = bTime + 100;
			blueButtonState = digitalRead(blueButPin);
			redButtonState = digitalRead(redButPin);
			if (blueButtonState == HIGH && redButtonState == HIGH)
			  {
				menuOpts = 1;
				lastBlueButtonState = HIGH;
				lastRedButtonState = HIGH;
			  }
			lcd.clear();
			lcd.setCursor(0, 0);
			boot2 = boot2 + '.';
			lcd.print(boot2);
		  }
  
		bTime = 0;
		delay(1000);
		
		delayTime = (startDelay/100)*60;
		while(delayTime > 0)
		{
			delayTime2 = ((delayTime/60)*100);
			delayTime3 = (delayTime%60);
			lcd.setCursor(0, 0);
			lcd.print(delayTime2); // Fix this. Delay time display
			lcd.setCursor(0, 1);
			lcd.print(delayTime2); // Fix this. Delay time display
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
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Control the zone"); 
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

	if(blueScore >= 0)
								{
									bPos = 15;
									if(blueScore > 9)
									{
									bPos = 14;
									if(blueScore > 99)
									{
									bPos = 13;
									if(blueScore > 999)
									{
									bPos = 12;
									if(blueScore > 9999)
									{
									bPos = 11;
									}
									}
									}
									}
									
								}
	blueButtonState = digitalRead(blueButPin);
	redButtonState = digitalRead(redButPin);
  
/*~~~~~~~~~~~BLUE BUTTON HOLD~~~~~~~~~~~*/  
	if (blueButtonState == HIGH && lastBlueButtonState == LOW && isBlueCounting == false) 
	{
		firstBluePress = millis();
		lastBlueButtonState = HIGH;
		lcd.clear();
		lcd.setCursor(0, 1);
		lcd.print("1     HOLD     1");
		lcd.setCursor(0, 1);
		lcd.print("|     ....     |");
	}
  
  if(holdCount == 1)
  {
	 if (blueButtonState == HIGH && millis() - firstBluePress >= 1000 && isBlueCounting == false)
	{
		lcd.setCursor(0, 1);
		lcd.print("   ! CONTROL !  ");
		isBlueCounting = true;
		isRedCounting = false;
		firstBluePress = 0;
		lastBlueButtonState = LOW;
		digitalWrite(ledRed, LOW);
		digitalWrite(ledBlue, HIGH);
		lcd.clear();
	} 
  }
  
  if(holdCount == 5)
  {
	if (blueButtonState == HIGH && (millis() - firstBluePress >= 1000) && (millis() - firstBluePress <= 1050) && isBlueCounting == false)
	{
		lcd.setCursor(0, 1);
		lcd.print("2     HOLD     2");
		lcd.setCursor(0, 1);
		lcd.print("|    ......    |");
	}
  
	if (blueButtonState == HIGH && (millis() - firstBluePress >= 2000) && (millis() - firstBluePress <= 2050) && isBlueCounting == false)
	{
		lcd.setCursor(0, 1);
		lcd.print("3     HOLD     3");
		lcd.setCursor(0, 1);
		lcd.print("|   ........   |");
	}
  
	if (blueButtonState == HIGH && (millis() - firstBluePress >= 3000) && (millis() - firstBluePress <= 3050) && isBlueCounting == false)
	{
		lcd.setCursor(0, 1);
		lcd.print("4     HOLD     4");
		lcd.setCursor(0, 1);
		lcd.print("|  ..........  |");
	}
	
	if (blueButtonState == HIGH && millis() - firstBluePress >= 4000 && isBlueCounting == false)
	{
		lcd.setCursor(0, 1);
		lcd.print("5     HOLD     5");
		lcd.setCursor(0, 1);
		lcd.print("|..............|");
	}
	
	if (blueButtonState == HIGH && millis() - firstBluePress >= 5000 && isBlueCounting == false)
	{
		lcd.setCursor(0, 1);
		lcd.print("   ! CONTROL !  ");
		isBlueCounting = true;
		isRedCounting = false;
		firstBluePress = 0;
		lastBlueButtonState = LOW;
		digitalWrite(ledRed, LOW);
		digitalWrite(ledBlue, HIGH);
		lcd.clear();
	}
  }
  
	if (blueButtonState == LOW && lastBlueButtonState == HIGH && isBlueCounting == false) 
	{
		lastBlueButtonState = LOW;
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("RED         BLUE");
		lcd.setCursor(0, 1);
		lcd.print(redScore);
		lcd.setCursor(bPos, 1);
		lcd.print(blueScore);
		firstBluePress = 0;
	}

	  
/*~~~~~~~~~~~RED BUTTON HOLD~~~~~~~~~~~*/
	if (redButtonState == HIGH && lastRedButtonState == LOW && isRedCounting == false) 
	{
		firstRedPress = millis();
		lastRedButtonState = HIGH;
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("1     HOLD     1");
		lcd.setCursor(0, 1);
		lcd.print("|     ....     |");
	}

	if(holdCount == 1)
	{
		if (redButtonState == HIGH && millis() - firstRedPress >= 1000 && isRedCounting == false)
		{
			lcd.setCursor(0, 1);
			lcd.print("   ! CONTROL !  ");
			isRedCounting = true;
			isBlueCounting = false;
			firstRedPress = 0;
			lastRedButtonState = LOW;
			digitalWrite(ledRed, HIGH);
			digitalWrite(ledBlue, LOW);
			lcd.clear();
		}
	}
		
	
	if(holdCount == 5)
	{
		if (redButtonState == HIGH && (millis() - firstRedPress >= 1000) && (millis() - firstRedPress <= 1050) && isRedCounting == false)
		{
			lcd.setCursor(0, 0);
			lcd.print("2     HOLD     2");
			lcd.setCursor(0, 1);
			lcd.print("|    ......    |");
		}
	  
		if (redButtonState == HIGH && (millis() - firstRedPress >= 2000) && (millis() - firstRedPress <= 2050) && isRedCounting == false)
		{
			lcd.setCursor(0, 1);
			lcd.print("3     HOLD     3");
			lcd.setCursor(0, 1);
			lcd.print("|   ........   |");
		}
	  
		if (redButtonState == HIGH && (millis() - firstRedPress >= 3000) && (millis() - firstRedPress <= 3050) && isRedCounting == false)
		{
			lcd.setCursor(0, 1);
			lcd.print("4     HOLD     4");
			lcd.setCursor(0, 1);
			lcd.print("|  ..........  |");
		}
		
		if (redButtonState == HIGH && millis() - firstRedPress >= 4000 && isRedCounting == false)
		{
			lcd.setCursor(0, 1);
			lcd.print("5     HOLD     5");
			lcd.setCursor(0, 1);
			lcd.print("|..............|");
		}

		if (redButtonState == HIGH && millis() - firstRedPress >= 5000 && isRedCounting == false)
		{
			lcd.setCursor(0, 1);
			lcd.print("   ! CONTROL !  ");
			isRedCounting = true;
			isBlueCounting = false;
			firstRedPress = 0;
			lastRedButtonState = LOW;
			digitalWrite(ledRed, HIGH);
			digitalWrite(ledBlue, LOW);
			lcd.clear();
		}
	}
	
	if (redButtonState == LOW && lastRedButtonState == HIGH && isRedCounting == false) 
	{
		lastRedButtonState = LOW;
		lcd.clear();
		lcd.print("RED         BLUE");
		lcd.setCursor(0, 1);
		lcd.print(redScore);
		lcd.setCursor(bPos, 1);
		lcd.print(blueScore); 
		firstRedPress = 0;
	}

	unsigned long curBlueMillis = millis();

/*~~~~~~~~~~~BLUE COUNTING~~~~~~~~~~~*/	
	if (isBlueCounting == true && curBlueMillis - preBlueMillis > countRate)
	{
		blueScore = blueScore + 1;
		if(blueScore >= 0)
							{
								bPos = 15;
								if(blueScore > 9)
								{
								bPos = 14;
								if(blueScore > 99)
								{
								bPos = 13;
								if(blueScore > 999)
								{
								bPos = 12;
								if(blueScore > 9999)
								{
								bPos = 11;
								}
								}
								}
								}
								
							}
		
		if ((redButtonState == HIGH && isRedCounting == false) || (blueButtonState == HIGH && isBlueCounting == false))
		{	
		}
		else
		{
		lcd.setCursor(0, 0);
		lcd.print("RED         BLUE");
		lcd.setCursor(bPos, 1);
		lcd.print(blueScore);
		lcd.setCursor(0, 1);
		lcd.print(redScore);	
		}			
		
		preBlueMillis = curBlueMillis;
		if (blueScore == winScore)
		{
			lcd.setCursor(0, 1);
			lcd.print(redScore);
			lcd.setCursor(bPos, 1);
			lcd.print(blueScore);
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
						lcd.setCursor(0, 1);
						lcd.print(redScore);
						lcd.setCursor(bPos, 1);
						lcd.print(blueScore);
						lastBlueButtonState = LOW;
						lastRedButtonState = LOW;
						digitalWrite(lowHorn, LOW);
						digitalWrite(highHorn, LOW);  
						digitalWrite(ledRed, LOW);
						digitalWrite(ledBlue, LOW);
						resetCount = 0;
					}
				}
				
				if(blueScore >= 0)
							{
								bPos = 15;
								if(blueScore > 9)
								{
								bPos = 14;
								if(blueScore > 99)
								{
								bPos = 13;
								if(blueScore > 999)
								{
								bPos = 12;
								if(blueScore > 9999)
								{
								bPos = 11;
								}
								}
								}
								}
								
							}
				lastBlueButtonState = blueButtonState;
				lastRedButtonState = redButtonState;
				digitalWrite(ledRed, LOW);
				digitalWrite(ledBlue, HIGH);
				lcd.setCursor(bPos, 1);
				lcd.print("Game Over");
				delay(100);
				lcd.setCursor(0, 1);
				lcd.print("                ");
				delay(100);
				lcd.setCursor(bPos, 1);
				lcd.print("Game Over");
				delay(100);
				lcd.setCursor(0, 1);
				lcd.print("                ");
				delay(100);
				lcd.setCursor(bPos, 1);
				lcd.print("Game Over");
				delay(100);
				lcd.setCursor(0, 1);
				lcd.print("                ");
				delay(100);
				digitalWrite(ledRed, LOW);
				digitalWrite(ledBlue, LOW);
				lcd.setCursor(0, 1);
						lcd.print(redScore);
						lcd.setCursor(bPos, 1);
						lcd.print(blueScore);
				delay(100);
				lcd.setCursor(0, 1);
				lcd.print("                ");
				delay(100);
				lcd.setCursor(0, 1);
						lcd.print(redScore);
						lcd.setCursor(bPos, 1);
						lcd.print(blueScore);
				delay(100);
				lcd.setCursor(0, 1);
				lcd.print("                ");
				delay(100);
				lcd.setCursor(0, 1);
						lcd.print(redScore);
						lcd.setCursor(bPos, 1);
						lcd.print(blueScore);
				delay(100);
				lcd.setCursor(0, 1);
				lcd.print("                ");
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
		if(blueScore >= 0)
							{
								bPos = 15;
								if(blueScore > 9)
								{
								bPos = 14;
								if(blueScore > 99)
								{
								bPos = 13;
								if(blueScore > 999)
								{
								bPos = 12;
								if(blueScore > 9999)
								{
								bPos = 11;
								}
								}
								}
								}
								
							}
		if ((redButtonState == HIGH && isRedCounting == false) || (blueButtonState == HIGH && isBlueCounting == false))
		{	
		}
		else
		{
		lcd.setCursor(0, 0);
		lcd.print("RED         BLUE");
		lcd.setCursor(bPos, 1);
		lcd.print(blueScore);
		lcd.setCursor(0, 1);
		lcd.print(redScore);	
		}

		if (redScore == winScore)
		{
			lcd.setCursor(0, 1);
			lcd.print(redScore);
			lcd.setCursor(bPos, 1);
			lcd.print(blueScore);
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
						lastBlueButtonState = LOW;
						lastRedButtonState = LOW;
						digitalWrite(lowHorn, LOW);
						digitalWrite(highHorn, LOW);  
						digitalWrite(ledRed, LOW);
						digitalWrite(ledBlue, LOW);
						resetCount = 0;
					}
				}
				
				if(blueScore >= 0)
							{
								bPos = 15;
								if(blueScore > 9)
								{
								bPos = 14;
								if(blueScore > 99)
								{
								bPos = 13;
								if(blueScore > 999)
								{
								bPos = 12;
								if(blueScore > 9999)
								{
								bPos = 11;
								}
								}
								}
								}
								
							}
				lastBlueButtonState = blueButtonState;
				lastRedButtonState = redButtonState;
				digitalWrite(ledRed, HIGH);
				digitalWrite(ledBlue, LOW);
				lcd.setCursor(0, 1);
				lcd.print("Game Over");
				delay(100);
				lcd.setCursor(0, 1);
				lcd.print("                ");
				delay(100);
				lcd.setCursor(0, 1);
				lcd.print("Game Over");
				delay(100);
				lcd.setCursor(0, 1);
				lcd.print("                ");
				delay(100);
				lcd.setCursor(0, 1);
				lcd.print("Game Over");
				delay(100);
				lcd.setCursor(0, 1);
				lcd.print("                ");
				delay(100);
				digitalWrite(ledRed, LOW);
				digitalWrite(ledBlue, LOW);
				lcd.setCursor(0, 1);
						lcd.print(redScore);
						lcd.setCursor(bPos, 1);
						lcd.print(blueScore);
				delay(100);
				lcd.setCursor(0, 1);
				lcd.print("                ");
				delay(100);
				lcd.setCursor(0, 1);
						lcd.print(redScore);
						lcd.setCursor(bPos, 1);
						lcd.print(blueScore);
				delay(100);
				lcd.setCursor(0, 1);
				lcd.print("                ");
				delay(100);
				lcd.setCursor(0, 1);
						lcd.print(redScore);
						lcd.setCursor(bPos, 1);
						lcd.print(blueScore);
				delay(100);
				lcd.setCursor(0, 1);
				lcd.print("                ");
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
