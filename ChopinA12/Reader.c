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
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: May 01 2024
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

/*
***********************************************************
* Function name: readerCreate
* Purpose: Creates the buffer reader according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: Svillen Ranev / Paulo Sousa
* History/Versions: S22
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to reader)
* Algorithm: Allocation of memory according to inicial (default) values.
* TODO ......................................................
*	- Adjust datatypes for your LANGUAGE.
*   - Use defensive programming
*	- Check boundary conditions
*	- Check flags.
*************************************************************
*/

BufferPointer readerCreate(chopin_intg size, chopin_intg increment, chopin_intg mode) {
	BufferPointer readerPointer;
	// check for negative size or increment
	if ((size < 0) || (increment < 0)) {
		return NULL;
	}
	// allocate memory for the buffer
	readerPointer = (BufferPointer)calloc(1, sizeof(Buffer));
	if (!readerPointer)
		return NULL;
	
	// Initializes the histogram to 0's
	for (int i = 0; i < NCHAR; i++) {
		readerPointer->histogram[i] = 0;
	}
	// assigns size
	if (size != 0)
		readerPointer->size = size;
	else
		readerPointer->size = READER_DEFAULT_SIZE;
	// allocates memory for content
	readerPointer->content = (chopin_string)malloc(readerPointer->size);
	if (!readerPointer->content) {
		return NULL;
	}
	// assigns increment
	if (increment != 0) {
		readerPointer->increment = increment;
	}
	else {
		readerPointer->increment = READER_DEFAULT_INCREMENT;
		mode = MODE_FIXED;
	}
	// assigns mode
	if (mode == MODE_ADDIT || mode == MODE_FIXED || mode == MODE_MULTI) {
		readerPointer->mode = mode;
	}
	else
		return NULL;
	// sets emp flag to 1 and all others to 0
	readerPointer->flags = READER_DEFAULT_FLAG;
	readerPointer->flags |= FLAG_EMP;
	// set the first character to \0
	if (readerPointer->content)
		readerPointer->content[0] = READER_TERMINATOR;
	// initialize positions
	readerPointer->position.wrte = 0;
	readerPointer->position.mark = 0;
	readerPointer->position.read = 0;
	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

BufferPointer readerAddChar(BufferPointer const readerPointer, chopin_char ch) {
	chopin_string tempReader = NULL;
	chopin_intg newSize = 0;
	if (!readerPointer) {
		return NULL;
	}
	// rel flag set to 0
	readerPointer->flags &= ~FLAG_REL;
	// check if chars are valid
	if ((chopin_byte)ch >= NCHAR) {
		readerPointer->numReaderErrors++;
		return NULL;
	}
	// check if wrte position is beyond size, if it is set the full flag
	if (readerPointer->position.wrte * (chopin_intg)sizeof(chopin_char) < readerPointer->size) {
		readerPointer->flags &= ~FLAG_FUL;
	} else {
		readerPointer->flags |= FLAG_FUL;
		// depending on mode set a new size
		switch (readerPointer->mode) {
		case MODE_FIXED:
			return NULL;
		case MODE_ADDIT:
			newSize = readerPointer->size + readerPointer->increment;
			if (newSize < 0 || newSize > READER_MAX_SIZE) {
				return NULL;
			}
			break;
		case MODE_MULTI:
			newSize = readerPointer->size * readerPointer->increment;
			if (newSize < 0 || newSize > READER_MAX_SIZE) {
				return NULL;
			}
			break;
		default:
			return NULL;
		}
		// allocate memory for new size
		tempReader = (chopin_string)realloc(readerPointer->content, newSize);
		if (!tempReader) {
			return NULL;
		}
		// if pointer location has moved, set the rel flag
		if (tempReader != readerPointer->content) {
			readerPointer->flags |= FLAG_REL;
		}

		readerPointer->content = tempReader;
		readerPointer->size = newSize;
	}
	// record the character to content and add it to the histogram
	readerPointer->content[readerPointer->position.wrte++] = ch;
	readerPointer->histogram[ch]++;
	// set the empty flag to 0
	readerPointer->flags &= ~FLAG_EMP;
	return readerPointer;
}

/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_boln readerClear(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return CHOPIN_FALSE;
	}
	// reset flags and positions
	readerPointer->flags &= READER_DEFAULT_FLAG;
	readerPointer->position.wrte = readerPointer->position.mark = readerPointer->position.read = 0;
	return CHOPIN_TRUE;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_boln readerFree(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer || !readerPointer->content) {
		return CHOPIN_FALSE;
	}
	/* TO_DO: Free pointers */
	free(readerPointer->content);
	free(readerPointer);
	return CHOPIN_TRUE;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_boln readerIsFull(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		return CHOPIN_FALSE;
	}
	// if ful flag is set return true
	if ((readerPointer->flags & FLAG_FUL) != 0) {
		return CHOPIN_TRUE;
	}
	return CHOPIN_FALSE;
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_boln readerIsEmpty(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		return CHOPIN_FALSE;
	}
	/* TO_DO: Check flag if buffer is EMP */
	if ((readerPointer->flags & FLAG_EMP) != 0) {
		return CHOPIN_TRUE;
	}
	return CHOPIN_FALSE;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_boln readerSetMark(BufferPointer const readerPointer, chopin_intg mark) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		return CHOPIN_FALSE;
	}
	if (mark < 0 || mark > readerGetPosWrte(readerPointer)) {
		return CHOPIN_FALSE;
	}
	/* TO_DO: Adjust mark */
	readerPointer->position.mark = mark;
	return CHOPIN_TRUE;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_intg readerPrint(BufferPointer const readerPointer) {
	chopin_intg cont = 0;
	chopin_char c;
	if (!readerPointer) {
		return READER_ERROR;
	}
	c = readerGetChar(readerPointer);
	
	/* TO_DO: Check flag if buffer EOB has achieved */
	while (cont < readerPointer->position.wrte) {
		if (c > NCHAR) {
			return READER_ERROR;
		}
		cont++;
		printf("%c", c);
		c = readerGetChar(readerPointer);
	}
	return cont;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_intg readerLoad(BufferPointer const readerPointer, FILE* const fileDescriptor) {
	chopin_intg size = 0;
	chopin_char c;
	if (!readerPointer) {
		return READER_ERROR;
	}
	c = (chopin_char)fgetc(fileDescriptor);
	while (!feof(fileDescriptor)) {
		if (!readerAddChar(readerPointer, c)) {
			ungetc(c, fileDescriptor);
			return READER_ERROR;
		}
		c = (char)fgetc(fileDescriptor);
		size++;
	}
	if (!fileDescriptor) {
		return READER_ERROR;
	}
	return size;
}


/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_boln readerRecover(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return CHOPIN_FALSE;
	}
	readerPointer->position.mark = 0;
	readerPointer->position.read = 0;
	return CHOPIN_TRUE;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_boln readerRetract(BufferPointer const readerPointer) {
	if (!readerPointer || (readerPointer->position.read < 1)) {
		return CHOPIN_FALSE;
	}
	readerPointer->position.read--;
	return CHOPIN_TRUE;
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_boln readerRestore(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return CHOPIN_FALSE;
	}
	readerPointer->position.read = readerPointer->position.mark;
	return CHOPIN_TRUE;
}


/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_char readerGetChar(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR;
	}
	if (readerPointer->position.wrte == readerPointer->position.read) {
		readerPointer->flags |= FLAG_END;
		return READER_TERMINATOR;
	}
	else {
		readerPointer->flags &= ~FLAG_END;
	}
	if (readerPointer->position.wrte>0)
		return readerPointer->content[readerPointer->position.read++];
	return READER_TERMINATOR;
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_string readerGetContent(BufferPointer const readerPointer, chopin_intg pos) {
	if (!readerPointer) {
		return NULL;
	}
	if (pos < 0 || pos > readerGetPosWrte(readerPointer)) {
		return NULL;
	}
	/* TO_DO: Return content (string) */
	return readerPointer->content + pos;
}



/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_intg readerGetPosRead(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR;
	}
	return readerPointer->position.read;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_intg readerGetPosWrte(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR;
	}
	return readerPointer->position.wrte;
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_intg readerGetPosMark(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR;
	}
	return readerPointer->position.mark;
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_intg readerGetSize(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR;
	}
	return readerPointer->size;
}

/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_intg readerGetInc(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR;
	}
	return readerPointer->increment;
}

/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_intg readerGetMode(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR;
	}
	return readerPointer->mode;
}


/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_byte readerGetFlags(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR;
	}
	return readerPointer->flags;
}



/*
***********************************************************
* Function name: readerPrintStat
* Purpose: Shows the char statistic.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value: (Void)
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_void readerPrintStat(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return;
	}
	/* TO_DO: Print the histogram */
	for (int i = 0; i < NCHAR; i++) {
		if (readerPointer->histogram[i] != 0) {
			printf("B[%c]=%d, ", i, readerPointer->histogram[i]);
		}
	}
	printf("\n");
}

/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
chopin_intg readerNumErrors(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR;
	}
	return readerPointer->numReaderErrors;
}

/*
***********************************************************
* Function name: readerChecksum
* Purpose: Sets the checksum of the reader (4 bits).
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	[None]
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

chopin_void readerChecksum(BufferPointer readerPointer) {
	chopin_char total = 0;
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		return;
	}
	/* TO_DO: Adjust the checksum to flags */
	for (int i = 0; i < readerPointer->position.wrte; i++) {
		total += readerPointer->content[i];
	}
	total = total << 4;
	readerPointer->flags |= total;
	return;
}
