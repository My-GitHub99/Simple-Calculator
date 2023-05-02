/*
 * app.c
 *
 *  Created on: Apr 12, 2023
 *      Author: ahmed bahaa
 */

/* user story
 User stroy 1
 * user types the first arithmatic operation>>>> characters gets disaplyed on LCD
 * user then presses '=">>>> computes the answer and displays it on LCD

 User story 2
 * user types the first arithmatic operation>>>> characters gets disaplyed on LCD
 * user then presses '=">>>> computes the answer and displays it on LCD
 * the user presses any operation button to pefrom an additional arithamtic operation on the answer then presses '='


 * User Story 3
 * user types the first arithmatic operation>>>> characters gets disaplyed on LCD
 * user then presses '=">>>> computes the answer and displays it on LCD
 * user presses any number to start a new operation

 User story 4
 * user types the first arithmatic operation>>>> characters gets disaplyed on LCD
 * deletes some characters and then enters another characters and presses '='

 **/
#include "app.h"
#include "../HAL/LCD/lcd.h"
#include "../HAL/KEYPAD/keypad.h"
#include <avr/io.h>
#include"../common_macros.h"
#include <util/delay.h> /* For the delay functions */
#include <stdlib.h>


 uint8 count=0; // global counter for entered characters

void displayKeypadOnLcd(uint8 keypadCharacter)
{

	LCD_moveCursor(0,count);
	// if clear button is pressed, clear a character on LCD
	if(keypadCharacter==127 && count>0)
	{
		LCD_moveCursor(0,--count);
		LCD_displayCharacter(' ');
	}
	else if(count == 0 && (keypadCharacter==127 || keypadCharacter=='='))
	{
		// print nothing
	}
	else if(keypadCharacter=='=')
	{
		//print nothing
	}
	else if(keypadCharacter >=0 && keypadCharacter <=9)
	{
		LCD_intgerToString(keypadCharacter);
		count++;
	}
	else
	{
	LCD_displayCharacter(keypadCharacter);
	count++;
	}

}


void fillOperationArray(uint8 keypadCharacter, uint8 operationArray[16])
{
static uint8 i=0;


if (keypadCharacter >=0 && keypadCharacter <=9 )
{
	operationArray[i]=keypadCharacter+'0';
	i++;
}

else if(keypadCharacter == '+' || keypadCharacter == '-' || keypadCharacter == '*' || keypadCharacter == '%'  )
{
	operationArray[i]=keypadCharacter;
	i++; // start a new string
}
/*if clear button is pressed on keypad*/

else if (keypadCharacter== 127)
{
	operationArray[--i]='\0';
}
else
{
// do nothing
}
operationArray[i]='\0';
}


void printAnswer(sint32 answer)
{
	LCD_moveCursor(1,3);
	LCD_intgerToString(answer);
}


Operation_state_EN operationIsValid(uint8 *arr)
{
	uint8 *ptr=arr;
	// if only an operator is entered
	if( (ptr[0]== '*') || ptr[0]== '%' || (( (ptr[0]== '+') || (ptr[0]== '-') ) &&  count==1 ))
		{
			return SYNTAX_ERROR ;
		}

	// checking the right and left operand of every operator
	else
	{
		for(int i=1;arr[i]!='\0';i++)
		{
		if(ptr[i]== '+' || ptr[i] == '-' || ptr[i]== '*' || ptr[i]== '%')
		{
		if(! ((ptr[i+1]>='0') && (ptr[i+1]<='9')) )
		   {
			 return SYNTAX_ERROR ;
		   }
		  else if(! ((ptr[i-1]>='0') && (ptr[i-1]<='9')) )
		   {
			 return SYNTAX_ERROR ;
		   }
		  else
		   {
			// do nothing
		   }
		}
		else
		{
          // do nothing
		}
	 }
	}
	return VALID_OPERATION;
}

sint32 computeAnswer(uint8 *arr)
{
	uint8 *ptr=arr;
	sint32 answer=0;
    int leftIndex=15;
    int rightIndex=0;
    int operationCounts=0;
	int i=0; // for looping on operation array
	int j=0; // for filling the operands
	uint8 extractLeftOperand[17]={'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','\0'};
	uint8 extractRightOperand[17]= {'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
	/* only a number is entered and */


	/* loop on the operation elements to find multiplication or division operators first*/
	while(ptr[i]!='\0')
	  {
		if(ptr[i] =='*' || ptr[i] =='%' )
		{
			operationCounts++;
	/* extract the left operand of the operation*/
			j=i-1;
			while(ptr[j]>='0' && ptr[j]<='9' && j>=0)
			{
				extractLeftOperand[leftIndex]=ptr[j];
				leftIndex--;
				j--;
			}
	/*extract the right operand of the operation*/
			j=i+1;
			while(ptr[j]>='0' && ptr[j]<='9' && ptr[j]!='\0')
			{
				extractRightOperand[rightIndex]=ptr[j];
				rightIndex++;
				j++;
			}
   /*execute operation*/
			if (ptr[i] =='*')
			{
			answer= atoi((const char *)extractLeftOperand) * atoi((const char *)extractRightOperand);
			}
			else
			{
			answer =  atoi((const char *)extractLeftOperand) / atoi((const char *)extractRightOperand) ;
			}

		}
		i++; /* incerement counter if no operation is found*/
	  }

	i=0;
	while(ptr[i]!='\0')
	  {
		if(ptr[i] =='+' || ptr[i] =='-' )
				{
			        operationCounts++;
					j=i-1;
					while(ptr[j]>='0' && ptr[j]<='9')
					{
						extractLeftOperand[leftIndex]=ptr[j];
						leftIndex--;
						j--;
					}

					j=i+1;
					while(ptr[j]>='0' && ptr[j]<='9')
					{
						extractRightOperand[rightIndex]=ptr[j];
						rightIndex++;
						j++;
					}
					if (ptr[i] =='+')
					{
					answer= atoi((const char *)extractLeftOperand) + atoi((const char *)extractRightOperand);
					}
					else
					{
					answer= atoi((const char *)extractLeftOperand) - atoi((const char *)extractRightOperand);
					}

				}
				i++;
	  }
	if(operationCounts==0)
	{
		answer=atoi((const char *)arr);
		return answer;
	}
	else
	{
		return answer;
	}

}


void test_displayKeypadOnLCD(void)
{
	uint8 keypad= KEYPAD_getPressedKey();
	displayKeypadOnLcd(keypad);
	_delay_ms(300); /* Press time */
}

void test_FillOperationArray(void)
{
	static uint8 count=0;
	static uint8 array[16];
	uint8 keypad= KEYPAD_getPressedKey();
	count++;
	fillOperationArray(keypad,array);
	_delay_ms(300); /* Press time */
	LCD_clearScreen();
	LCD_moveCursor(0,0);
	for(int i=0; i<count;i++)
	{
		LCD_displayCharacter(array[i]);

	}

}

void test_OperationIsValid(void)
{
	static uint8 count=0;
		static uint8 array[16];
		uint8 keypad= KEYPAD_getPressedKey();
		count++;
		fillOperationArray(keypad,array);
		_delay_ms(300); /* Press time */
		LCD_clearScreen();
		LCD_moveCursor(0,0);
		for(int i=0; i<count;i++)
		{
			LCD_displayCharacter(array[i]);

		}
	if (operationIsValid(array)==SYNTAX_ERROR)
	{
		LCD_moveCursor(1,0);
		LCD_displayString("SYNTAX ERROR");
	}
	else
	{
		LCD_moveCursor(1,0);
		LCD_displayString("VALID");
	}

}

void test_ComputeAnswer(void)
{
	static uint8 count=0;
	static uint8 array[16];
    uint32 result=0;
	uint8 keypad= KEYPAD_getPressedKey();
		count++;
		fillOperationArray(keypad,array);
		_delay_ms(300); /* Press time */
		LCD_clearScreen();
		LCD_moveCursor(0,0);
		for(int i=0; i<count;i++)
		{
			LCD_displayCharacter(array[i]);

		}
		LCD_moveCursor(1,9);
	if(keypad=='=')
	{
	result = computeAnswer(array);
	 LCD_intgerToString(result);
	}

}
