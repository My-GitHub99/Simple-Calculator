/*
 * app.h
 *
 *  Created on: Apr 12, 2023
 *      Author: user
 */

#ifndef APPLICATION_APP_H_
#define APPLICATION_APP_H_

#include "../MCAL/GPIO/gpio.h"
#include "../std_types.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
typedef enum OPERATION_state_EN{
	VALID_OPERATION, SYNTAX_ERROR
}Operation_state_EN;

Operation_state_EN operationIsValid(uint8 *arr);


/*
 * Description :
 * computes the typed arithmetic operation
 * 1. takes an input array of 16 strings (16 pointer to char) that symbols the typed strings
 * 2. return the arithmetic result
 */
sint32 computeAnswer(uint8 *arr);

/*
 * Description :
 * prints the aritmatic result on the LCD
 */
void printAnswer(sint32 answer);

/*
 * Description :
 * displays the typed KEYPAD characters on LCD
 */
void displayKeypadOnLcd(uint8 keypadCharacter);

/*
 * Description :
 * constructs an array of srtrings reasambles the required arithmatic oepration, each strings reseambles a digit or an operator
 * input: the kepad character pressed by user
 * return: an array of stings
 */

void fillOperationArray(uint8 keypadCharacter, uint8 operationArray[16]);

/*******************************************************************************
 *                      Test Functions Definitions                                  *
 *******************************************************************************/
void test_OperationIsValid(void);
void test_displayKeypadOnLCD(void);
void test_FillOperationArray(void);
void test_PrintAnswer(void);
void test_ComputeAnswer(void);



#endif /* APPLICATION_APP_H_ */
