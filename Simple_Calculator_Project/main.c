/*
 * main.c
 *
 *  Created on: Apr 12, 2023
 *      Author: user
 */


#include "./Application/app.h"
#include "./HAL/KEYPAD/keypad.h"
#include <util/delay.h> /* For the delay functions */
#include "./HAL/LCD/lcd.h"


int main ()
{
	static uint8 operationArray[16]={'\0'};
	uint32 answer=0;
	uint8 keypadCharacter;
	uint8 resultIsShownFlag=0;
	LCD_init();
	while(1)
	{
		keypadCharacter= KEYPAD_getPressedKey();
		if( resultIsShownFlag==1 && keypadCharacter==127 )
		{
			LCD_moveCursor(1,0);
			LCD_displayString("                                      ");
			resultIsShownFlag=0;
			continue;
		}

		displayKeypadOnLcd(keypadCharacter);
		_delay_ms(250); /* Press time */
		fillOperationArray(keypadCharacter,operationArray);

		if(keypadCharacter=='=' && operationArray[0]!='\0')
		{
			resultIsShownFlag=1;
			if (operationIsValid(operationArray)==SYNTAX_ERROR)
			{
			LCD_moveCursor(1,0);
			LCD_displayString("SYNTAX ERROR");
			}

			else
			{
				answer=computeAnswer(operationArray);
				printAnswer(answer);

			}

		}

	//test_displayKeypadOnLCD();
	//test_FillOperationArray();
	//test_OperationIsValid();
	//test_ComputeAnswer();
	}
}
