/*
 * front.h
 *
 *  Created on: Apr 3, 2019
 *      Author: kangh (provided .h file)
 */

#ifndef FRONT_H_
#define FRONT_H_

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
//editing below code value from 20 to 27
#define ASSIGN_OP 27 //changed value to 27
//end edit
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
//this is new
#define SEM_OP 28
//end new

int lex();

#endif /* FRONT_H_ */
