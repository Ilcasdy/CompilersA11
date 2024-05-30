/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2024
* Author: Sean Bradbury
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
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: May 01 2024
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

/* TIP: Do not change pragmas, unless necessary .......................................*/
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer reader) */
enum READER_MODE {
	MODE_FIXED = 'f', /* Fixed mode (constant size) */
	MODE_ADDIT = 'a', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'm'  /* Multiplicative mode (constant increment to be multiplied) */
};

/* Constants about controls (not need to change) */
#define READER_ERROR		(-1)						/* General error message */
#define READER_TERMINATOR	'\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (chopin) .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE	INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */
#define READER_DEFAULT_INCREMENT	10		/* default increment factor */

/* Add your bit-masks constant definitions here - Defined for BOA */
/* BITS                                (7654.3210) */
#define READER_DEFAULT_FLAG 0x00 	/* (0000.0000)_2 = (000)_10 */

#define FLAG_END 0b00001000 /* Hex: 0x08 Dec: 8 */
#define FLAG_REL 0b00000100 /* Hex: 0x04 Dec: 4 */
#define FLAG_FUL 0b00000010 /* Hex: 0x02 Dec: 2 */
#define FLAG_EMP 0b00000001 /* Hex: 0x01 Dec: 1 */

#define NCHAR				128			/* Chars from 0 to 127 */

#define CHARSEOF			(-1)		/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (chopin) .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	chopin_intg mark;			/* the offset to the mark position (in chars) */
	chopin_intg read;			/* the offset to the get a char position (in chars) */
	chopin_intg wrte;			/* the offset to the add chars (in chars) */
} Position;

/* Buffer structure */
typedef struct bufferReader {
	chopin_string	content;			/* pointer to the beginning of character array (character buffer) */
	chopin_intg		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	chopin_intg		increment;			/* character array increment factor */
	chopin_intg		mode;				/* operational mode indicator */
	chopin_byte		flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position		position;				/* Offset / position field */
	chopin_intg		histogram[NCHAR];	/* Statistics of chars */
	chopin_intg		numReaderErrors;	/* Number of errors from Reader */
} Buffer, *BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
BufferPointer	readerCreate		(chopin_intg, chopin_intg, chopin_intg);
BufferPointer	readerAddChar		(BufferPointer const, chopin_char);
chopin_boln		readerClear		    (BufferPointer const);
chopin_boln		readerFree		    (BufferPointer const);
chopin_boln		readerIsFull		(BufferPointer const);
chopin_boln		readerIsEmpty		(BufferPointer const);
chopin_boln		readerSetMark		(BufferPointer const, chopin_intg);
chopin_intg		readerPrint		    (BufferPointer const);
chopin_intg		readerLoad			(BufferPointer const, FILE* const);
chopin_boln		readerRecover		(BufferPointer const);
chopin_boln		readerRetract		(BufferPointer const);
chopin_boln		readerRestore		(BufferPointer const);
chopin_void		readerChecksum		(BufferPointer const);
/* Getters */
chopin_char		readerGetChar		(BufferPointer const);
chopin_string	readerGetContent	(BufferPointer const, chopin_intg);
chopin_intg		readerGetPosRead	(BufferPointer const);
chopin_intg		readerGetPosWrte	(BufferPointer const);
chopin_intg		readerGetPosMark	(BufferPointer const);
chopin_intg		readerGetSize		(BufferPointer const);
chopin_intg		readerGetInc		(BufferPointer const);
chopin_intg		readerGetMode		(BufferPointer const);
chopin_byte		readerGetFlags		(BufferPointer const);
chopin_void		readerPrintStat		(BufferPointer const);
chopin_intg		readerNumErrors		(BufferPointer const);

#endif
