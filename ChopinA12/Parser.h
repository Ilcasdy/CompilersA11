/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2023
* Author: Sean Bradbury
* Professors: Paulo Sousa
************************************************************
=---------------------------------------=
|  COMPILERS - ALGONQUIN COLLEGE (F23)  |
=---------------------------------------=
|              ....                     |
|          ........::.::::::.           |
|        .:........::.:^^^~~~:          |
|        :^^::::::^^^::^!7??7~^.        |
|       .:^~~^!77777~~7?YY?7??7^.       |
|       :.^~!??!^::::^^~!?5PY??!~.      |
|       ~!!7J~.:::^^^^~!!~~?G5J?~       |
|       :^~?!~7?Y57^^?PP5YJ!J5Y?:       |
|       .~!!.:^!7!:.:7JYYJ7~7Y7^        |
|       .~77..    . .~^:^^^~7?:         |
|       .^!^~:::.:^!7?~^~!77J:          |
|        ^^!Y~^^^^~?YJ77??7JJ^          |
|       .^7J?~^~~^~7??7??7JY?~:         |
|        ::^^~^7?!^~~!7???J?J7~:.       |
|         ^~~!.^7YPPPP5Y?7J7777~.       |
|        ..:~..:^!JPP5YJ?!777!^.        |
| .~?JJJJJJJJJJYYYYYPPPPPPPPPPPP5PPYY~  |
|  :!Y5GGG.___ YYYYYY__._.PPGGGGGG5!.   |
|   :!Y5G / __| ___ / _(_)__ _ PGP5.    |
|    :~75 \__ \/ _ \  _| / _` | 5?.     |
|     7~7 |___/\___/_| |_\__,_| Y5?.    |
|    .^~!~.....................P5YY7.   |
|   .:::::::::::::?JJJJYYYYYYYYYJJJJ7.  |
|                                       |
=---------------------------------------=
*/

/*
************************************************************
* File name: Parser.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: July 27 2024
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
************************************************************
*/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef READER_H_
#include "Reader.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token			lookahead;
extern BufferPointer	stringLiteralTable;
extern chopin_intg		line;
extern Token			tokenizer(chopin_void);
extern chopin_string		keywordTable[KWT_SIZE];
static chopin_intg		syntaxErrorNumber = 0;

#define LANG_WRTE		"write"
#define LANG_READ		"read"
#define LANG_MAIN		"main"
#define LANG_SHARP		"sharp"
#define LANG_FLAT		"flat"

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	KW_void,
	KW_dec,
	KW_int,
	KW_pitch,
	KW_duration,
	KW_velocity,
	KW_beat,
	KW_string,
	KW_boolean,
	KW_for,
	KW_if,
	KW_else,
	KW_while,
	KW_do,
	KW_Note,
	KW_Track,
	KW_Project,
	KW_return,
	KW_TRUE,
	KW_FALSE
};

/* TO_DO: Define the number of BNF rules */
#define NUM_BNF_RULES 255

/* Parser */
typedef struct parserData {
	chopin_intg parsHistogram[NUM_BNF_RULES];	/* Number of BNF Statements */
} ParserData, * pParsData;

/* Number of errors */
chopin_intg numParserErrors;

/* Scanner data */
ParserData psData;

/* Function definitions */
chopin_void startParser();
chopin_void matchToken(chopin_intg, chopin_intg);
chopin_void syncErrorHandler(chopin_intg);
chopin_void printError();
chopin_void printBNFData(ParserData psData);

/* List of BNF statements */
enum BNF_RULES {
	BNF_error,								/*  1 */
	BNF_aaePrime,							/*  2 */
	BNF_aaePrimePrime,						/*  3 */
	BNF_additiveArithmeticExpression,		/*  4 */
	BNF_comment,							/*  5 */
	BNF_data,								/*  6 */
	BNF_datatype,							/*  7 */
	BNF_flatStatement,						/*  8 */
	BNF_floatVarDeclaration,				/*  9 */
	BNF_optParameters,						/* 10 */
	BNF_maePrime,							/* 11 */
	BNF_maePrimePrime,						/* 12 */
	BNF_noteAttributions,					/* 13 */
	BNF_noteDeclaration,					/* 14 */
	BNF_noteExpression,						/* 15 */
	BNF_integerVarDeclaration,				/* 16 */
	BNF_parameters,							/* 17 */
	BNF_parameter,							/* 18 */
	BNF_parameterPrime,						/* 19 */
	BNF_optNoteAssignment,					/* 20 */
	BNF_optReturnData,						/* 21 */
	BNF_optStatements,						/* 22 */
	BNF_optVariableList,					/* 23 */
	BNF_outputStatement,					/* 24 */
	BNF_outputStatementPrime,				/* 25 */
	BNF_typeDeclaration,					/* 26 */
	BNF_program,							/* 27 */
	BNF_sharpStatement,						/* 28 */
	BNF_statement,							/* 29 */
	BNF_statements,							/* 30 */		
	BNF_statementsPrime,					/* 31 */
	BNF_stringVarDeclaration,				/* 32 */
	BNF_assignmentExpression,				/* 33 */
	BNF_selectionStatement,					/* 34 */
	BNF_optionalElseStatement,				/* 35 */
	BNF_iterationStatement,					/* 36 */
	BNF_inputStatement,						/* 37 */
	BNF_inputStatementPrime,				/* 38 */
	BNF_variableList,						/* 39 */
	BNF_variableListPrime,					/* 40 */
	BNF_multiplicativeArithmeticExpression,	/* 41 */
	BNF_primaryArithmeticExpression,		/* 42 */
	BNF_conditionalExpression,				/* 43 */
	BNF_logicalOrExpression,				/* 44 */
	BNF_logicalOrExpressionPrime,			/* 45 */
	BNF_logicalAndExpression,				/* 46 */
	BNF_logicalAndExpressionPrime,			/* 47 */
	BNF_logicalNotExpression,				/* 48 */
	BNF_relationalExpression,				/* 49 */
	BNF_relationalExpressionPrime,			/* 50 */
	BNF_trackExpression,					/* 51 */
	BNF_projectExpression,					/* 52 */
	BNF_trackAttributions,					/* 53 */
	BNF_trackDeclaration,					/* 54 */
	BNF_optTrackAssignment,					/* 55 */
	BNF_optNoteVariableList,				/* 56 */
	BNF_noteVariableListPrime,				/* 57 */
	BNF_projectAttributions,				/* 58 */
	BNF_projectDeclaration,					/* 59 */
	BNF_optProjectAssignment,				/* 60 */
	BNF_optTrackVariableList,				/* 61 */
	BNF_trackVariableListPrime				/* 62 */

};


/* TO_DO: Define the list of keywords */
static chopin_string BNFStrTable[NUM_BNF_RULES] = {
	"BNF_error",							/*  1 */
	"BNF_aaePrime",							/*  2 */
	"BNF_aaePrimePrime",					/*  3 */
	"BNF_additiveArithmeticExpression",		/*  4 */
	"BNF_comment",							/*  5 */
	"BNF_data",								/*  6 */
	"BNF_datatype",							/*  7 */
	"BNF_flatStatement",					/*  8 */
	"BNF_floatVarDeclaration",				/*  9 */
	"BNF_optParameters",					/* 10 */
	"BNF_integerVarDeclaration",			/* 11 */
	"BNF_maePrime",							/* 12 */
	"BNF_maePrimePrime",					/* 13 */
	"BNF_noteAttributions",					/* 14 */	
	"BNF_noteDeclaration",					/* 15 */
	"BNF_noteExpression",					/* 16 */
	"BNF_parameters",						/* 17 */
	"BNF_parameter",						/* 18 */
	"BNF_parameterPrime",					/* 19 */
	"BNF_optNoteAssignment",				/* 20 */
	"BNF_optReturnData",					/* 21 */
	"BNF_optStatements",					/* 22 */
	"BNF_optVariableList",					/* 23 */
	"BNF_outputStatement",					/* 24 */
	"BNF_outputStatementPrime",				/* 25 */
	"BNF_typeDeclaration",					/* 26 */		
	"BNF_program",							/* 27 */
	"BNF_sharpStatement",					/* 28 */
	"BNF_statement",						/* 29 */						
	"BNF_statements",						/* 30 */				
	"BNF_statementsPrime",					/* 31 */
	"BNF_stringVarDeclaration",				/* 32 */
	"BNF_assignmentExpression",				/* 33 */
	"BNF_selectionStatement",				/* 34 */
	"BNF_optionalElseStatement",			/* 35 */
	"BNF_iterationStatement",				/* 36 */
	"BNF_inputStatement",					/* 37 */
	"BNF_inputStatementPrime",				/* 38 */
	"BNF_variableList",						/* 39 */
	"BNF_variableListPrime",				/* 40 */
	"BNF_multiplicativeArithmeticExpression",/* 41 */
	"BNF_primaryArithmeticExpression",		/* 42 */
	"BNF_conditionalExpression",			/* 43 */
	"BNF_logicalOrExpression",				/* 44 */
	"BNF_logicalOrExpressionPrime",			/* 45 */
	"BNF_logicalAndExpression",				/* 46 */
	"BNF_logicalAndExpressionPrime",		/* 47 */
	"BNF_logicalNotExpression",				/* 48 */
	"BNF_relationalExpression",				/* 49 */
	"BNF_relationalExpressionPrime",		/* 50 */
	"BNF_trackExpression",					/* 51 */
	"BNF_projectExpression",				/* 52 */
	"BNF_trackAttributions",				/* 53 */
	"BNF_trackDeclaration",					/* 54 */
	"BNF_optTrackAssignment",				/* 55 */
	"BNF_optNoteVariableList",				/* 56 */
	"BNF_noteVariableListPrime",			/* 57 */
	"BNF_projectAttributions",				/* 58 */
	"BNF_projectDeclaration",				/* 59 */
	"BNF_optProjectAssignment",				/* 60 */
	"BNF_optTrackVariableList",				/* 61 */
	"BNF_trackVariableListPrime"			/* 62 */
};	

/* TO_DO: Place ALL non-terminal function declarations */
chopin_void aaePrime();							/*  1 */
chopin_void aaePrimePrime();					/*  2 */
chopin_void additiveArithmeticExpression();		/*  3 */
chopin_void comment();							/*  4 */
chopin_void data();								/*  5 */
chopin_void datatype();							/*  6 */
chopin_void flatStatement();					/*  7 */
chopin_void floatVarDeclaration();				/*  8 */
chopin_void integerVarDeclaration();			/*  9 */
chopin_void optParameters();					/* 11 */
chopin_void maePrime();							/* 12 */
chopin_void maePrimePrime();					/* 13 */
chopin_void noteAttributions();					/* 14 */
chopin_void noteDeclaration();					/* 15 */
chopin_void noteExpression();					/* 16 */
chopin_void parameters();						/* 17 */
chopin_void parameter();						/* 18 */
chopin_void parameterPrime();					/* 19 */
chopin_void optNoteAssignment();				/* 20 */
chopin_void optReturnData();					/* 21 */
chopin_void optStatements();					/* 22 */
chopin_void optVariableList();					/* 23 */
chopin_void outputStatement();					/* 24 */
chopin_void outputStatementPrime();				/* 25 */
chopin_void typeDeclaration();					/* 26 */
chopin_void program();							/* 27 */
chopin_void sharpStatement();					/* 28 */
chopin_void statement();						/* 29 */	
chopin_void statements();						/* 30 */	
chopin_void statementsPrime();					/* 31 */
chopin_void stringVarDeclaration();				/* 32 */
chopin_void assignmentExpression();				/* 33 */
chopin_void selectionStatement();				/* 34 */
chopin_void optionalElseStatement();			/* 35 */
chopin_void iterationStatement();				/* 36 */
chopin_void inputStatement();					/* 37 */
chopin_void inputStatementPrime();				/* 38 */
chopin_void variableList();						/* 39 */
chopin_void variableListPrime();				/* 40 */
chopin_void multiplicativeArithmeticExpression();/* 41 */
chopin_void primaryArithmeticExpression();		/* 42 */
chopin_void conditionalExpression();			/* 43 */
chopin_void logicalOrExpression();				/* 44 */
chopin_void logicalOrExpressionPrime();			/* 45 */
chopin_void logicalAndExpression();				/* 46 */
chopin_void logicalAndExpressionPrime();		/* 47 */
chopin_void logicalNotExpression();				/* 48 */
chopin_void relationalExpression();				/* 49 */
chopin_void relationalExpressionPrime();		/* 50 */
chopin_void trackExpression();					/* 51 */
chopin_void projectExpression();				/* 52 */
chopin_void trackAttributions();				/* 53 */
chopin_void trackDeclaration();					/* 54 */
chopin_void optTrackAssignment();				/* 55 */
chopin_void optNoteVariableList();				/* 56 */
chopin_void noteVariableListPrime();			/* 57 */
chopin_void projectAttributions();				/* 58 */
chopin_void projectDeclaration();				/* 59 */
chopin_void optProjectAssignment();				/* 60 */
chopin_void optTrackVariableList();				/* 61 */
chopin_void trackVariableListPrime();			/* 62 */
	
#endif
