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
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "    @@                             @@    �
# ECHO "    @@           %&@@@@@@@@@@@     @@    �
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    �
# ECHO "    @@      @& @   @ @       @     @@    �
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    �
# ECHO "    @@      & @ @  @@              @@    �
# ECHO "    @@       @/ @*@ @ @   @        @@    �
# ECHO "    @@           @@@@  @@ @ @      @@    �
# ECHO "    @@            /@@    @@@ @     @@    �
# ECHO "    @@     @      / /     @@ @     @@    �
# ECHO "    @@     @ @@   /@/   @@@ @      @@    �
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    �
# ECHO "    @@                             @@    �
# ECHO "    @@         S O F I A           @@    �
# ECHO "    @@                             @@    �
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "                                         "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Compilers.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: May 01 2024
* Professor: Paulo Sousa
* Purpose: This file defines the functions called by main function.
* Function list: mainReader(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

/* TO_DO: Adjust your language (cannot be "chopin") */

/* Language name */
#define STR_LANGNAME	"Chopin"

/* Logical constants - adapt for your language */
#define CHOPIN_TRUE  1
#define CHOPIN_FALSE 0

/*
------------------------------------------------------------
Data types definitions
NOTE: Some types may not be directly used by your language,
		but they can be necessary for conversions.
------------------------------------------------------------
*/


/* TO_DO: Define your typedefs */
typedef char			chopin_char;
typedef char*			chopin_string;
typedef int				chopin_intg;
typedef float			chopin_real;
typedef void			chopin_void;

typedef unsigned char	chopin_boln;
typedef unsigned char	chopin_byte;

typedef long			chopin_long;
typedef double			chopin_doub;

typedef float			chopin_pitch;
typedef float		chopin_duration;
typedef float			chopin_beat;
typedef float		chopin_velocity;

struct Note {
	chopin_pitch tone;
	chopin_duration length;
	chopin_beat time;
	chopin_velocity volume;
};

typedef struct Note		Note;

struct Track {
	chopin_string name;
	Note *roll;
};

typedef struct Track	Track;

struct Project {
	chopin_string name;
	chopin_string author;
	chopin_real bpm;
	Track *song;
};

typedef struct Project	Project;

/*
------------------------------------------------------------
Programs:
1: Reader - invokes MainReader code
2: Scanner - invokes MainScanner code
3: Parser - invokes MainParser code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_READER	= 'r',
	PGM_SCANNER = 's',
	PGM_PARSER	= 'p'
};

/*
------------------------------------------------------------
Main functions signatures
(Code will be updated during next assignments)
------------------------------------------------------------
*/
chopin_intg mainReader(chopin_intg argc, chopin_string* argv);
chopin_intg mainScanner(chopin_intg argc, chopin_string* argv);
chopin_intg mainParser(chopin_intg argc, chopin_string* argv);
/* 
TO_DO: Include later mainScaner (A22) and mainParser (A32)
*/
chopin_void printLogo();

#endif
