/* front.c - a lexical analyzer system for simple arithmetic expressions */
/*
 * author : Tarik Muzughi
 * date : 7-22-19
 * desc: progrom builds on lexical analyzer allowing multiple statements and comments
 */
#include <stdio.h>
#include <ctype.h>

#include "front.h"
//input.txt
/* Global Variable */
int nextToken;

/* Local Variables */
static int charClass;
static char lexeme [100];
static char nextChar;
/*
 * nextNextChar, lastChar, and lastLastChar added
 * to keep track of input values before and
 * after nextChar through iterations
 */
static char nextNextChar;
static char lastChar;
static char lastLastChar;
static int waxOn = 0;
static int lexLen;
static FILE *in_fp;
static FILE *win_fp; //easiest way I could think of to use getc twice in same function

/* Local Function declarations */
static void addChar();
static void getChar();
static void getNonBlank();
static void getGetChar();



/******************************************************/
/* main driver */
int main()
{
    /* Open the input data file and process its contents */

    if ((in_fp = fopen("input.txt", "r")) == NULL) {
        printf("ERROR - cannot open input.txt \n");
    } else {
    	win_fp = fopen("input.txt", "r"); //add separate FILE variable for nextNextChar
    	getGetChar(); //this is the extra iteration for nextNextChar
        getChar();
        do {
            lex();
        } while (nextToken != EOF);
    }

    return 0;
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the
 * token */
static int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
            //cases set for ';' and '='
        case ';':
        	addChar();
        	nextToken = SEM_OP;
        	break;
        case '=':
        	addChar();
        	nextToken = ASSIGN_OP;
        	break;
        default:
        	/*  |
        	 *  |  This allows "illegal" characters such as '.'
        	 *  v   to slip through the main process without issue
        	 *  	as long as they are within comments
        	 */
        	if (nextChar == EOF){
            addChar();
            nextToken = EOF;
        	}
            break;
    }
    return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
static void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else {
        printf("Error - lexeme is too long \n");
    }
}
/*
 * function added to iterate nextNextChar(from inside getChar)
 */
static void getGetChar(){
	nextNextChar = getc(win_fp);
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its
 * character class */
static void getChar() {
	lastLastChar = lastChar; // <-- Keeping lastLastChar and lastChar on
		lastChar = nextChar; // <---- track with nextChar
    if ((nextChar = getc(in_fp)) != EOF) {
    	getGetChar(); //iterate nextNextChar with nextChar
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = UNKNOWN;
    } else {
        charClass = EOF;
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace
 * character */
static void getNonBlank() {
    while (isspace(nextChar)) getChar();
}


/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
/*
 * Reset waxOn "switch" because end of comments found
 */
    if (lastLastChar == '*' && lastChar == '/'){
            	   waxOn = 0;
}
    	getNonBlank();
    switch (charClass) {
        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENT;
            break;

        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        /* Parentheses and operators */
        case UNKNOWN:
        	/*
        	 * Beginning of comments found, set "switch" waxOn
        	 */
        	if (nextChar == '/' && nextNextChar == '*'){
        		waxOn = 1;
        	}
            lookup(nextChar);
            getChar();

            break;

        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    } /* End of switch */
/*
 * display check
 */
    if (waxOn == 0)
    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);




    return nextToken;
} /* End of function lex */
