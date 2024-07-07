/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2024
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S24)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@           %&@@@@@@@@@@@     @@    ”
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    ”
# ECHO "    @@      @& @   @ @       @     @@    ”
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    ”
# ECHO "    @@      & @ @  @@              @@    ”
# ECHO "    @@       @/ @*@ @ @   @        @@    ”
# ECHO "    @@           @@@@  @@ @ @      @@    ”
# ECHO "    @@            /@@    @@@ @     @@    ”
# ECHO "    @@     @      / /     @@ @     @@    ”
# ECHO "    @@     @ @@   /@/   @@@ @      @@    ”
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@         S O F I A           @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "                                         "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Scanner.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/*
----------------------------------------------------------------
TO_DO: Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
chopin_intg line;								/* Current line number of the source code */
extern chopin_intg errorNumber;				/* Defined in platy_st.c - run-time error number */

extern chopin_intg stateType[NUM_STATES];
extern chopin_string keywordTable[KWT_SIZE];
extern PTR_ACCFUN finalStateTable[NUM_STATES];
extern chopin_intg transitionTable[NUM_STATES][CHAR_CLASSES];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */
 /* TO_DO: Follow the standard and adjust datatypes */

chopin_intg startScanner(BufferPointer psc_buf) {
	/* TO_DO: Start histogram */
	for (chopin_intg i=0; i<NUM_TOKENS;i++)
		scData.scanHistogram[i] = 0;
	/* Basic scanner initialization */
	/* in case the buffer has been read previously  */
	readerRecover(psc_buf);
	readerClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states 
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(chopin_void) {

	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	chopin_char c, d;	/* input symbol */
	chopin_intg state = 0;		/* initial state of the FSM */
	chopin_intg lexStart;		/* start offset of a lexeme in the input char buffer (array) */
	chopin_intg lexEnd;		/* end offset of a lexeme in the input char buffer (array)*/

	chopin_intg lexLength;		/* token length */
	chopin_intg i;				/* counter */
	///chopin_char newc;			// new char

	while (1) { /* endless loop broken by token returns it will generate a warning */
		c = readerGetChar(sourceBuffer);
		

		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/

		switch (c) {

		/* Cases for spaces */
		case ' ':
		case '\t':
		case '\f':
			break;
		case '\n':
			line++;
			break;

		/* Cases for symbols */
		case ';':
			currentToken.code = EOS_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '(':
			currentToken.code = LPR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case ')':
			currentToken.code = RPR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '{':
			currentToken.code = LBR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '}':
			currentToken.code = RBR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '=':
			if (readerGetChar(sourceBuffer) != '=') {
				currentToken.code = EQU_T;
				scData.scanHistogram[currentToken.code]++;
				readerRetract(sourceBuffer);
				return currentToken;
			}
			else {
				currentToken.code = REL_T;
				currentToken.attribute.relationalOperator = OP_EQ;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			}
		case '<':
			if (readerGetChar(sourceBuffer) != '=') {
				currentToken.code = REL_T;
				currentToken.attribute.relationalOperator = OP_LT;
				scData.scanHistogram[currentToken.code]++;
				readerRetract(sourceBuffer);
				return currentToken;
			}
			else {
				currentToken.code = REL_T;
				currentToken.attribute.relationalOperator = OP_LTE;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			}
		case '>':
			if (readerGetChar(sourceBuffer) != '=') {
				currentToken.code = REL_T;
				currentToken.attribute.relationalOperator = OP_GT;
				scData.scanHistogram[currentToken.code]++;
				readerRetract(sourceBuffer);
				return currentToken;
			}
			else {
				currentToken.code = REL_T;
				currentToken.attribute.relationalOperator = OP_GTE;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			}
		case '!':
			if (readerGetChar(sourceBuffer) != '=') {
				currentToken.code = LOG_T;
				currentToken.attribute.logicalOperator = OP_NOT;
				scData.scanHistogram[currentToken.code]++;
				readerRetract(sourceBuffer);
				return currentToken;
			}
			else {
				currentToken.code = REL_T;
				currentToken.attribute.relationalOperator = OP_NE;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			}
		case '+':
			currentToken.code = ARI_T;
			currentToken.attribute.arithmeticOperator = OP_ADD;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '*':
			currentToken.code = ARI_T;
			currentToken.attribute.arithmeticOperator = OP_MUL;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '/':
			if (readerGetChar(sourceBuffer) != '/') {
				currentToken.code = ARI_T;
				currentToken.attribute.arithmeticOperator = OP_DIV;
				scData.scanHistogram[currentToken.code]++;
				readerRetract(sourceBuffer);
				return currentToken;
			}
			else {
				readerRetract(sourceBuffer);
				currentToken = runAutomata(c);
				return currentToken;
			}
		case '-':
			d = readerGetChar(sourceBuffer);
			if (d != '.' && !isdigit(d)) {
				currentToken.code = ARI_T;
				currentToken.attribute.arithmeticOperator = OP_SUB;
				scData.scanHistogram[currentToken.code]++;
				readerRetract(sourceBuffer);
				return currentToken;
			}
			else {
				readerRetract(sourceBuffer);
				currentToken = runAutomata(c);
				return currentToken;
			}
		case ',':
			currentToken.code = COM_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case '.':
			d = readerGetChar(sourceBuffer);
			if (isdigit(d)) {
				readerRetract(sourceBuffer);
				currentToken = runAutomata(c);
				return currentToken;
			}
			else {
				readerRetract(sourceBuffer);
				currentToken.code = PER_T;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			}
		case '&':
			if (readerGetChar(sourceBuffer) == '&') {
				currentToken.code = LOG_T;
				currentToken.attribute.logicalOperator = OP_AND;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			}
			else {
				readerRetract(sourceBuffer);
				currentToken.code = ERR_T;
				scData.scanHistogram[currentToken.code]++;
				strcpy(currentToken.attribute.errLexeme, "Invalid character, use '&&' for AND");
				return currentToken;
			}
		case '|':
			if (readerGetChar(sourceBuffer) == '|') {
				currentToken.code = LOG_T;
				currentToken.attribute.logicalOperator = OP_OR;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			}
			else {
				readerRetract(sourceBuffer);
				currentToken.code = ERR_T;
				scData.scanHistogram[currentToken.code]++;
				strcpy(currentToken.attribute.errLexeme, "Invalid character, use '||' for OR");
				return currentToken;
			}

		/* Cases for END OF FILE */
		case CHARSEOF0:
			currentToken.code = SEOF_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken;
		case CHARSEOF255:
			currentToken.code = SEOF_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.seofType = SEOF_255;
			return currentToken;

		/* ------------------------------------------------------------------------
			Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
			Note: Part 2 must follow Part 1 to catch the illegal symbols
			-----------------------------------------------------------------------
		*/

		default: // general case
			currentToken = runAutomata(c);
			return currentToken;
		} // switch

	} //while

} // tokenizer


/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */

chopin_intg nextState(chopin_intg state, chopin_char c) {
	chopin_intg col;
	chopin_intg next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != FS);
	if (DEBUG)
		if (next == FS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */

/*/*    [A-z],[0-9],  [/],  [#],  [_],  [.],  ["],  [\n],  [-], other
	     L(0), D(1), S(2), H(3), U(4), B(5), Q(6),  N(7), M(8), O(9)    */

chopin_intg nextClass(chopin_char c) {
	chopin_intg val = -1;
	switch (c) {
	case CHRCOL2:
		val = 2;
		break;
	case CHRCOL3:
		val = 3;
		break;
	case CHRCOL4:
		val = 4;
		break;
	case CHRCOL5:
		val = 5;
		break;
	case CHRCOL6:
		val = 6;
		break;
	case CHRCOL7:
		val = 7;
		break;
	case CHRCOL8:
		val = 8;
		break;
	case CHARSEOF0:
	case CHARSEOF255:
		val = 5;
		break;
	default:
		if (isalpha(c))
			val = 0;
		else if (isdigit(c))
			val = 1;
		else
			val = 9;
	}
	return val;
}

/*
 ************************************************************
 * Acceptance State Function CMT
 *		Function responsible to identify CMT (comments).
 ***********************************************************
 */

Token funcCMT(chopin_string lexeme) {
	Token currentToken = { 0 };
	chopin_intg i = 0, len = (chopin_intg)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == '\n')
			line++;
	}
	currentToken.code = CMT_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


 /*
  ************************************************************
  * Acceptance State Function IL
  *		Function responsible to identify IL (integer literals).
  * - It is necessary respect the limit (ex: 2-byte integer in C).
  * - In the case of larger lexemes, error shoul be returned.
  * - Only first ERR_LEN characters are accepted and eventually,
  *   additional three dots (...) should be put in the output.
  ***********************************************************
  *
  /* TO_DO: Adjust the function for IL */

Token funcIL(chopin_string lexeme) {
	Token currentToken = { 0 };
	chopin_long tlong;
	if (lexeme[0] != '\0' && strlen(lexeme) > NUM_LEN) {
		currentToken = (*finalStateTable[ESNR])(lexeme);
	}
	else {
		tlong = atol(lexeme);
		if (tlong >= SHRT_MIN && tlong <= SHRT_MAX) {
			currentToken.code = INL_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.intValue = (chopin_intg)tlong;
		}
		else {
			currentToken = (*finalStateTable[ESNR])(lexeme);
		}
	}
	return currentToken;
}
/*
************************************************************
*Acceptance State Function FL
* Function responsible to identify FL (Float literals)
* limited by NUM_LEN
***********************************************************
*/

Token funcFL(chopin_string lexeme) {
	Token currentToken = { 0 };
	chopin_real tfloat;
	if (lexeme[0] != '\0' && strlen(lexeme) > NUM_LEN) {
		currentToken = (*finalStateTable[ESNR])(lexeme);
	}
	else {
		tfloat = strtof(lexeme, NULL);
		if (tfloat >= SHRT_MIN && tfloat <= SHRT_MAX) {
			currentToken.code = FL_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.floatValue = tfloat;
		}
		else {
			currentToken = (*finalStateTable[ESNR])(lexeme);
		}
	}
	return currentToken;
}

/*
 ************************************************************
 * Acceptance State ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */

Token funcID(chopin_string lexeme) {

	Token currentToken = { 0 };
	size_t length = strlen(lexeme);
	lexeme[length] = '\0';
	// check if lexeme is keyword
	chopin_intg kwindex = -1, j = 0;
	for (chopin_intg j = 0; j < KWT_SIZE; j++) {
		if (!strcmp(lexeme, keywordTable[j])) {
			kwindex = j;
		}
	}
	if (kwindex != -1) {
		currentToken.code = KW_T;
		scData.scanHistogram[currentToken.code]++;
		currentToken.attribute.codeType = kwindex;
	}
	else {
		currentToken.code = ID_T;
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
		scData.scanHistogram[currentToken.code]++;
	}
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table 
 *   (stringLiteralTable). You need to include the literals in 
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */

Token funcSL(chopin_string lexeme) {
	Token currentToken = { 0 };
	chopin_intg i = 0, len = (chopin_intg)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == '\n')
			line++;
		if (!readerAddChar(stringLiteralTable, lexeme[i])) {
			currentToken.code = RTE_T;
			scData.scanHistogram[currentToken.code]++;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
	if (!readerAddChar(stringLiteralTable, CHARSEOF0)) {
		currentToken.code = RTE_T;
		scData.scanHistogram[currentToken.code]++;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}
	currentToken.code = STR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}

/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */

Token funcErr(chopin_string lexeme) {
	Token currentToken = { 0 };
	chopin_intg i = 0, len = (chopin_intg)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = CHARSEOF0;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == '\n')
			line++;
	currentToken.code = ERR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

chopin_void printToken(Token t) {
	extern chopin_string keywordTable[]; /* link to keyword table in */
	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		numScannerErrors++;
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case ID_T:
		printf("ID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T\t\t%s\n", readerGetContent(stringLiteralTable, (chopin_intg)t.attribute.codeType));
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case CMT_T:
		printf("CMT_T\n");
		break;
	case EOS_T:
		printf("EOS_T\n");
		break;
	case ARI_T:
		printf("ARI_T\t\t%s\n", arithmeticStringTable[t.attribute.arithmeticOperator] );
		break;
	case EQU_T:
		printf("EQU_T\n");
		break;
	case REL_T:
		printf("REL_T\t\t%s\n", relationalStringTable[t.attribute.relationalOperator]);
		break;
	case LOG_T:
		printf("LOG_T\t\t%s\n", logicalStringTable[t.attribute.logicalOperator]);
		break;
	case COM_T:
		printf("COM_T\n");
		break;
	case INL_T:
		printf("INL_T\t\t%d\n", t.attribute.intValue);
		break;
	case FL_T:
		printf("FL_T\t\t%f\n", t.attribute.floatValue);
		break;
	case PER_T:
		printf("PER_T\n");
		break;
	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

/*
 ************************************************************
 * The function prints statistics of tokens
 * Param:
 *	- Scanner data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
chopin_void printScannerData(ScannerData scData) {
	/* Print Scanner statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_TOKENS; cont++) {
		if (scData.scanHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", tokenStrTable[cont], "]=", scData.scanHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}
/*
************************************************************
* This function runs the automata if none of the symbols
* match the first part of tokenizer, or if it is determined
* that the symbol is part of the automata rather than a 
* single character token.
***********************************************************
*/

Token runAutomata(chopin_string c) {

	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	chopin_intg state = 0;		/* initial state of the FSM */
	chopin_intg lexStart;		/* start offset of a lexeme in the input char buffer (array) */
	chopin_intg lexEnd;			/* end offset of a lexeme in the input char buffer (array)*/
	chopin_intg i;

	chopin_intg lexLength;		/* token length */
	state = nextState(state, c);
	lexStart = readerGetPosRead(sourceBuffer) - 1;
	readerSetMark(sourceBuffer, lexStart);
	int pos = 0;
	while (stateType[state] == NOFS) {
		c = readerGetChar(sourceBuffer);
		state = nextState(state, c);
		pos++;
	}
	if (stateType[state] == FSWR)
		readerRetract(sourceBuffer);
	lexEnd = readerGetPosRead(sourceBuffer);
	lexLength = lexEnd - lexStart;
	lexemeBuffer = readerCreate((chopin_intg)lexLength + 2, 0, MODE_FIXED);
	if (!lexemeBuffer) {
		fprintf(stderr, "Scanner error: Can not create buffer\n");
		exit(1);
	}
	readerRestore(sourceBuffer);
	for (i = 0; i < lexLength; i++)
		readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
	readerAddChar(lexemeBuffer, READER_TERMINATOR);
	currentToken = (*finalStateTable[state])(readerGetContent(lexemeBuffer, 0));
	readerRestore(lexemeBuffer);
	return currentToken;
}
