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
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 20   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

#define NUM_TOKENS 20

enum TOKENS {
	ERR_T,		/*  0: Error token */
	ID_T,		/*  1: Identifier token */
	INL_T,		/*  2: Integer literal token */
	STR_T,		/*  3: String literal token */
	LPR_T,		/*  4: Left parenthesis token */
	RPR_T,		/*  5: Right parenthesis token */
	LBR_T,		/*  6: Left brace token */
	RBR_T,		/*  7: Right brace token */
	KW_T,		/*  8: Keyword token */
	EOS_T,		/*  9: End of statement (semicolon) */
	RTE_T,		/* 10: Run-time error token */
	SEOF_T,		/* 11: Source end-of-file token */
	CMT_T,		/* 12: Comment token */
	ARI_T,		/* 13: Arithmetic token */
	EQU_T,		/* 14: Equal sign token */
	COM_T,		/* 15: Comma token*/
	REL_T,		/* 16: Relational token*/
	FL_T,		/* 17: Float token */
	LOG_T,		/* 18: Logical token */
	PER_T		/* 19: Period token */
};

static chopin_string tokenStrTable[NUM_TOKENS] = {
	"ERR_T",
	"ID_T",
	"INL_T",
	"STR_T",
	"LPR_T",
	"RPR_T",
	"LBR_T",
	"RBR_T",
	"KW_T",
	"EOS_T",
	"RTE_T",
	"SEOF_T",
	"CMT_T",
	"ARI_T",
	"EQU_T",
	"COM_T",
	"REL_T",
	"FL_T",
	"LOG_T",
	"PER_T"
};

typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_GTE, OP_LT, OP_LTE } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

static chopin_string arithmeticStringTable[4] = {
	"+", "-", "*", "/"
};
static chopin_string relationalStringTable[6] = {
	"==", "!=", ">", ">=", "<", "<="
};
static chopin_string logicalStringTable[3] = {
	"&&", "||", "!"
};

typedef union TokenAttribute {
	chopin_intg codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	chopin_intg intValue;				/* integer literal attribute (value) */
	chopin_intg keywordIndex;			/* keyword index in the keyword table */
	chopin_intg contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	chopin_real floatValue;				/* floating-point literal attribute (value) */
	chopin_char idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	chopin_char errLexeme[ERR_LEN + 1];	/* error token attribute */
} TokenAttribute;

typedef struct idAttibutes {
	chopin_byte flags;			/* Flags information */
	union {
		chopin_intg intValue;				/* Integer value */
		chopin_real floatValue;			/* Float value */
		chopin_string stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	chopin_intg code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	chopin_intg scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner:
 *  LPR_T, RPR_T, LBR_T, RBR_T, EOS_T, SEOF_T and special chars used for tokenis include _, & and ' */

/* These constants will be used on nextClass */
#define CHRCOL2 '/'
#define CHRCOL3 '#'
#define CHRCOL4 '_'
#define CHRCOL5 '.'
#define CHRCOL6 '"'
#define CHRCOL7 '\n'
#define CHRCOL8 '-'

#define ESNR	18		/* Error state with no retract */
#define ESWR	19		/* Error state with retract */
#define FS		20		/* Illegal state */

#define NUM_STATES		20
#define CHAR_CLASSES	10

static chopin_intg transitionTable[NUM_STATES][CHAR_CLASSES] = {
/*    [A-z],[0-9],  [/],  [#],  [_],  [.],  ["],  [\n],  [-], other
	   L(0), D(1), S(2), H(3), U(4), B(5), Q(6),  N(7), M(8), O(9)    */
	{     8,   10,    1, ESNR, ESNR,   13,   16,  ESNR,   12, ESNR},	// S0: NOFS
	{  ESNR, ESNR,    2, ESNR, ESNR, ESNR, ESNR,  ESNR, ESNR, ESNR},	// S1: NOFS
	{     2,    2,    4,    2,    2,    2,	  2,     3,    2,    2},	// S2: NOFS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,    FS,   FS,   FS},	// S3: FSNR (SLC)
	{     4,    4,    5,    4,    4,    4,	  4,     4,    4,    4},	// S4: NOFS
	{     5,    5,    6,    5,    5,    5,	  5,     5,    5,    5},	// S5: NOFS
	{     6,    6,    7,    6,    6,    6,	  6,     6,    6,    6},	// S6: NOFS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,    FS,   FS,   FS},	// S7: FSNR (MLC)
	{     8,    8,    9,    8,    8,    9,	  9,     9,    9,    9},	// S8: NOFS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,    FS,   FS,   FS},    // S9: FSWR (VID|MID|KEY)
	{    11,   10,   11,   11,   11,   13,	 11,    11,   11,   11},	// S10: NOFS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,    FS,   FS,   FS},    // S11: FSWR (IL)
	{  ESNR,   10, ESNR, ESNR, ESNR,   13, ESNR,  ESNR, ESNR, ESNR},	// S12: NOFS
	{  ESNR,   14, ESNR, ESNR, ESNR, ESNR, ESNR,  ESNR, ESNR, ESNR},	// S13: NOFS
	{    15,   14,   15,   15,   15,   15,	 15,    15,   15,   15},	// S14: NOFS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,    FS,   FS,   FS},    // S15: FSWR (FL)
	{    16,   16,   16,   16,   16,   16,	 17,    16,   16,   16},	// S16: NOFS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,    FS,   FS,   FS},    // S17: FSNR (SL)
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,    FS,   FS,   FS},    // S18: FSNR (ERR)
};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* list of acceptable states */
static chopin_intg stateType[NUM_STATES] = {
	NOFS, /* 00 */
	NOFS, /* 01 */
	NOFS, /* 02 */
	FSNR, /* 03 (SLC) */
	NOFS, /* 04 */
	NOFS, /* 05 */
	NOFS, /* 06 */
	FSNR, /* 07 (MLC) */
	NOFS, /* 08 */
	FSWR, /* 09 (VID|MID|KEY) */
	NOFS, /* 10 */
	FSWR, /* 11 (IL) */
	NOFS, /* 12 */
	NOFS, /* 13 */
	NOFS, /* 14 */
	FSWR, /* 15 (FL) */
	NOFS, /* 16 */
	FSNR, /* 17 (SL) */
	FSNR  /* 18 (ERR) */
};

/* Static (local) function  prototypes */
chopin_intg			startScanner(BufferPointer psc_buf);
static chopin_intg	nextClass(chopin_char c);					/* character class function */
static chopin_intg	nextState(chopin_intg, chopin_char);		/* state machine function */
chopin_void			printScannerData(ScannerData scData);
Token				tokenizer(chopin_void);

Token				runAutomata(chopin_string c);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

typedef Token(*PTR_ACCFUN)(chopin_string lexeme);

/* Declare accepting states functions */
Token funcSL	(chopin_string lexeme);
Token funcIL	(chopin_string lexeme);
Token funcFL	(chopin_string lexeme);
Token funcID	(chopin_string lexeme); 
Token funcCMT   (chopin_string lexeme);
Token funcErr	(chopin_string lexeme);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	NULL,		/* -    [02] */
	funcCMT,	/* SLC	[03] */
	NULL,		/* -    [04] */
	NULL,		/* -    [05] */
	NULL,		/* -    [06] */
	funcCMT,	/* MLC  [07] */
	NULL,		/* -    [08] */
	funcID,		/* ID   [09] */
	NULL,		/* -    [10] */
	funcIL,		/* IL   [11] */
	NULL,		/* -    [12] */
	NULL,		/* -    [13] */
	NULL,		/* -    [14] */
	funcFL,		/* -    [15] */
	NULL,		/* -    [16] */
	funcSL,		/* -    [17] */
	funcErr,	/* ERR1 [18] */
	funcErr		/* ERR2 [19] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

#define KWT_SIZE 20

/* list of keywords */
static chopin_string keywordTable[KWT_SIZE] = {
	"return",	/* KW00 */
	"dec",		/* KW01 */
	"int",		/* KW02 */
	"pitch",	/* KW03 */
	"duration",	/* KW04 */
	"velocity",	/* KW05 */
	"beat",		/* KW06 */
	"string",	/* KW07 */
	"boolean",	/* KW08 */
	"for",		/* KW09 */
	"if",		/* KW10 */
	"else",		/* KW11 */
	"while",	/* KW12 */
	"do",		/* KW13 */
	"Note",		/* KW14 */	
	"Track",	/* KW15 */
	"Project",	/* KW16 */
	"void",		/* KW17 */
	"TRUE",		/* KW18 */
	"FALSE"		/* KW19 */
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT '\t'  /* Tabulation */

/* Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	chopin_char indentationCharType;
	chopin_intg indentationCurrentPos;

} LanguageAttributes;

/* Number of errors */
chopin_intg numScannerErrors;

/* Scanner data */
ScannerData scData;

#endif
