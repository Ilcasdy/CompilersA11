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
* File name: Parser.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: July 27 2024
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/* Parser data */
extern ParserData psData; /* BNF statistics */

/*
************************************************************
 * Process Parser
 ***********************************************************
 */

chopin_void startParser() {
	chopin_intg i = 0;
	for (i = 0; i < NUM_BNF_RULES; i++) {
		psData.parsHistogram[i] = 0;
	}
	/* Proceed parser */
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
chopin_void matchToken(chopin_intg tokenCode, chopin_intg tokenAttribute) {
	chopin_intg matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType != tokenAttribute && tokenAttribute != NO_ATTR)
			matchFlag = 0;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
chopin_void syncErrorHandler(chopin_intg syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
chopin_void printError() {
	extern numParserErrors;			/* link to number of errors (defined in Parser.h) */
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("*ERROR*: %s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case STR_T:
		printf("STR_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("KW_T: %s\n", keywordTable[t.attribute.codeType]);
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
	case EOS_T:
		printf("NA\n");
		break;
	case INL_T:
		printf("INL_T\t%d\n", t.attribute.intValue);
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
		numParserErrors++; // Updated parser error
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> main& { <opt_statements> }
 * FIRST(<program>)= {CMT_T, ID_T (main&), SEOF_T}.
 ***********************************************************
 */
chopin_void program() {
	/* Update program statistics */
	psData.parsHistogram[BNF_program]++;
	/* Program code */
	switch (lookahead.code) {
	case CMT_T:
		comment();
	case KW_T:
		switch (lookahead.attribute.codeType) {
		// match a datatype keyword
		case KW_void:
		case KW_int:
		case KW_dec:
		case KW_string:
		case KW_pitch:
		case KW_duration:
		case KW_velocity:
		case KW_beat:
		case KW_Note:
		case KW_Track:
		case KW_Project:
			datatype();
			// main function
			if (strncmp(lookahead.attribute.idLexeme, LANG_MAIN, 4) == 0) {
				matchToken(ID_T, NO_ATTR);
				matchToken(LPR_T, NO_ATTR);
				optParameters();
				matchToken(RPR_T, NO_ATTR);
				matchToken(LBR_T, NO_ATTR);
				optStatements();
				matchToken(KW_T, KW_return);
				optReturnData();
				matchToken(EOS_T, NO_ATTR);
				matchToken(RBR_T, NO_ATTR);
			}
			else {
				printError();
			}
			break;
		default:
			printError();
		}
		break;
	case SEOF_T:
		; // Empty
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

/*
 ************************************************************
 * comment
 * BNF: comment
 * FIRST(<comment>)= {CMT_T}.
 ***********************************************************
 */
chopin_void comment() {
	psData.parsHistogram[BNF_comment]++;
	matchToken(CMT_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Comment parsed");
}

chopin_void datatype() {
	psData.parsHistogram[BNF_datatype]++;
	matchToken(KW_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Datatype parsed");
}



/* TO_DO: Continue the development (all non-terminal functions) */

/*
 ************************************************************
 * Optional parameters
 * BNF: <opt_parameters> -> <parameters> | ϵ
 * FIRST(<opt_parameters>) = { ϵ , KW_int, KW_dec, KW_string,
								KW_pitch, KW_duration, KW_beat,
								KW_velocity, KW_Note, KW_Track,
								KW_Project }
 ***********************************************************
 */
chopin_void optParameters() {
	psData.parsHistogram[BNF_optParameters]++;
	switch (lookahead.code) {
	case CMT_T:
		comment();
		return;
	case KW_T:
		switch (lookahead.attribute.codeType) {
			// match a datatype keyword
		case KW_void:
		case KW_int:
		case KW_dec:
		case KW_string:
		case KW_pitch:
		case KW_duration:
		case KW_velocity:
		case KW_beat:
		case KW_Note:
		case KW_Track:
		case KW_Project:
			parameters();
			break;
		default:
			printError();
		}
		break;
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional parameters parsed");
}

/*
 ************************************************************
 * parameters
 * BNF: <parameters> -> <parameter> <parameterPrime>
 * FIRST(<parameters>) = { KW_int, KW_dec, KW_string,
								KW_pitch, KW_duration, KW_beat,
								KW_velocity, KW_Note, KW_Track,
								KW_Project }
 ***********************************************************
 */
chopin_void parameters() {
psData.parsHistogram[BNF_parameters]++;
	parameter();
	parameterPrime();
	printf("%s%s\n", STR_LANGNAME, ": parameters parsed");
}

/*
 ************************************************************
 * parameterPrime
 * BNF: <parameterPrime> -> COM_T <parameter> <parameterPrime> | ϵ 
 * FIRST(<parameterPrime>) = { ϵ , COM_T }
 ***********************************************************
 */
chopin_void parameterPrime() {
	psData.parsHistogram[BNF_parameterPrime]++;
	switch (lookahead.code) {
	case COM_T:
		matchToken(COM_T, NO_ATTR);
		parameter();
		parameterPrime();
		printf("%s%s\n", STR_LANGNAME, ": parameterPrime parsed");
		break;
	default:
		;
	}	
}

/*
 ************************************************************
 * parameter
 * BNF: <parameter> -> COM_T <datatype><data> | ϵ
 * FIRST(<parameter>) = { KW_int, KW_dec, KW_string,
								KW_pitch, KW_duration, KW_beat,
								KW_velocity, KW_Note, KW_Track,
								KW_Project }
 ***********************************************************
 */
chopin_void parameter() {
	psData.parsHistogram[BNF_parameter]++;
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
			// match a datatype keyword
		case KW_int:
		case KW_dec:
		case KW_string:
		case KW_pitch:
		case KW_duration:
		case KW_velocity:
		case KW_beat:
		case KW_Note:
		case KW_Track:
		case KW_Project:
			datatype();
			matchToken(ID_T, NO_ATTR);
			break;
		default:
			printError();
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": parameter parsed");
}
/*
 ************************************************************
 * Optional statement
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<opt_statements>) = { ϵ , KW_T, ID_T, KW_return}
 ***********************************************************
 */
chopin_void optStatements() {
	psData.parsHistogram[BNF_optStatements]++;
	switch (lookahead.code) {
	case CMT_T:
		comment();
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case KW_return:
			break;
		case KW_if:
		case KW_do:
		case KW_while:
		case KW_int:
		case KW_dec:
		case KW_string:
		case KW_pitch:
		case KW_beat:
		case KW_velocity:
		case KW_Note:
		case KW_Track:
		case KW_Project:
		case KW_duration:
			statements();
			break;
		}
		break;
	case ID_T:
			statements();
			break;
	
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*
 ************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *		KW_T(KW_while) }
 ***********************************************************
 */
chopin_void statements() {
	psData.parsHistogram[BNF_statements]++;
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
 ************************************************************
 * Statements Prime
 * BNF: <statementsPrime> -> <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T, 
 *		KW_T(KW_if), KW_T(KW_while) }
 ***********************************************************
 */
chopin_void statementsPrime() {
	psData.parsHistogram[BNF_statementsPrime]++;
	if (lookahead.code == KW_T && lookahead.attribute.codeType == KW_return) {
		return;
	}
	if (lookahead.code == RBR_T) {
		return;
	}
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": StatementsPrime parsed");
}

/*
 ************************************************************
 * Single statement
 * BNF: <statement> -> <assignment statement> | <selection statement> |
 *	<iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if), KW_T(KW_while)
 *			 }
 ***********************************************************
 */
chopin_void statement() {
	psData.parsHistogram[BNF_statement]++;
	switch (lookahead.code) {
	case CMT_T:
		comment();
		return;
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case KW_int:
			integerVarDeclaration();
			break;
		case KW_dec:
		case KW_pitch:
		case KW_beat:
		case KW_duration:
		case KW_velocity:
			floatVarDeclaration();
			break;
		case KW_string:
			stringVarDeclaration();
			break;
		case KW_if:
			selectionStatement();
			break;
		case KW_do:
		case KW_while:
			iterationStatement();
			break;
		case KW_Note:
			noteDeclaration();
			break;
		case KW_Track:
			trackDeclaration();
			break;
		case KW_Project:
			projectDeclaration();
			break;
		case EOS_T:
			break;
		case KW_return:
			break;
		default:
			printError();
		}
		break;
	case ID_T:
		if ((strncmp(lookahead.attribute.idLexeme, LANG_READ, 4)) == 0) {
			inputStatement();
		}
		else if ((strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 5)) == 0) {
			outputStatement();
		}
		else if ((strncmp(lookahead.attribute.idLexeme, LANG_SHARP, 5)) == 0) {
			sharpStatement();
		}
		else if ((strncmp(lookahead.attribute.idLexeme, LANG_FLAT, 4)) == 0) {
			flatStatement();
		}
		else {
			matchToken(ID_T, NO_ATTR);
			matchToken(EQU_T, NO_ATTR);
			additiveArithmeticExpression();
		}
		break;
	default:
		printError();
	}
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*
 ************************************************************
 * Integer var declaration
 * BNF: <integer var declaration> -> KW_int <integer variable>;
 * FIRST(<integer var declaration>) = { KW_int }
 ***********************************************************
 */
chopin_void integerVarDeclaration() {
	psData.parsHistogram[BNF_integerVarDeclaration]++;
	matchToken(KW_T, NO_ATTR);
	matchToken(ID_T, NO_ATTR);
	
	if (lookahead.code == EQU_T) {
		matchToken(EQU_T, NO_ATTR);
		additiveArithmeticExpression();
		printf("%s%s\n", STR_LANGNAME, ": Integer variable declaration and assignment parsed");
		return;
	}
	printf("%s%s\n", STR_LANGNAME, ": Integer variable declaration parsed");
}
/*
 ************************************************************
 * Float var declaration
 * BNF: <float var declaration> -> KW_dec <float variable>;
 * FIRST(<float var declaration>) = { KW_dec }
 ***********************************************************
 */
chopin_void floatVarDeclaration() {
	psData.parsHistogram[BNF_floatVarDeclaration]++;
	matchToken(KW_T, NO_ATTR);
	matchToken(ID_T, NO_ATTR);
	
	if (lookahead.code == EQU_T) {
		matchToken(EQU_T, NO_ATTR);
		additiveArithmeticExpression();
		printf("%s%s\n", STR_LANGNAME, ": Float variable declaration and assignment parsed");
		return;
	}
	printf("%s%s\n", STR_LANGNAME, ": Float variable declaration parsed");
}
/*
 ************************************************************
 * String var declaration
 * BNF: <string var declaration> -> KW_string ID_T;
 * FIRST(<float var declaration>) = { KW_string }
 ***********************************************************
 */
chopin_void stringVarDeclaration() {
	psData.parsHistogram[BNF_stringVarDeclaration]++;
	matchToken(KW_T, NO_ATTR);
	matchToken(ID_T, NO_ATTR);
	

	if (lookahead.code == EQU_T) {
		matchToken(EQU_T, NO_ATTR);
		matchToken(STR_T, NO_ATTR);
		printf("%s%s\n", STR_LANGNAME, ": String variable declaration and assignment parsed");
		return;
	}
	printf("%s%s\n", STR_LANGNAME, ": String variable declaration parsed");
}

chopin_void optReturnData() {
	psData.parsHistogram[BNF_optReturnData]++;
	switch (lookahead.code) {
	case ID_T:
		matchToken(ID_T, NO_ATTR);
		break;
	case FL_T:
		matchToken(FL_T, NO_ATTR);
		break;
	case INL_T:
		matchToken(INL_T, NO_ATTR);
		break;
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": optional return data parsed");
}
/*
 ************************************************************
 * additive arithmetic expression
 * BNF: <additive arithmetic expression> -> <multiplicative arithmetic expression> <aaePrime> 
 * FIRST(<output statement>) = { INL_T, FL_T, <integer variable>, <float type variable>, LPR_T }
 ***********************************************************
 */
chopin_void additiveArithmeticExpression() {
	psData.parsHistogram[BNF_additiveArithmeticExpression]++;
	multiplicativeArithmeticExpression();
	aaePrime();
	printf("%s%s\n", STR_LANGNAME, ": additive arithmetic expression parsed");
}
/*
 ************************************************************
 * additive arithmetic expression Prime
 * BNF: <additive arithmetic expression Prime> -> 
		<aaePrimePrime> 
		|<aaePrime> 
		| epsilon
 * FIRST(<output statement>) = { INL_T, FL_T, <integer variable>, <float type variable>, LPR_T }
 ***********************************************************
 */
chopin_void aaePrime() {
	psData.parsHistogram[BNF_aaePrime]++;
	switch (lookahead.code) {
	case ARI_T:
		switch (lookahead.attribute.codeType) {
		case OP_ADD:
		case OP_SUB:
			aaePrimePrime();
			aaePrime;
			break;
		default:
			;
		}
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": additive arithmetic expression Prime parsed");
}
/*
 ************************************************************
 * additive arithmetic expression Prime
 * BNF: <aaePrime> -> ARI_T(+) <mae> | ARI_T(-) <mae> | ϵ
 * FIRST(<output statement>) = { ARI_T }
 ***********************************************************
 */
chopin_void aaePrimePrime() {
	psData.parsHistogram[BNF_aaePrimePrime]++;
	switch (lookahead.code) {
	case ARI_T:
		if (lookahead.attribute.codeType == OP_ADD) {
			matchToken(ARI_T, NO_ATTR);
			multiplicativeArithmeticExpression();
		}
		else if (lookahead.attribute.codeType == OP_SUB) {
			matchToken(ARI_T, NO_ATTR);
			multiplicativeArithmeticExpression();
		}
		else {
			break;
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": additive arithmetic expression Prime Prime parsed");
}
/*
 ************************************************************
 * multiplicative arithmetic expression
 * BNF: <multiplicative arithmetic expression> -> <primary arithmetic expression> <maePrime>
 * FIRST(<output statement>) = { INL_T, FL_T, <integer variable>, <float type variable>, LPR_T }
 ***********************************************************
 */
chopin_void multiplicativeArithmeticExpression() {
	psData.parsHistogram[BNF_multiplicativeArithmeticExpression]++;
	primaryArithmeticExpression();
	maePrime();
	printf("%s%s\n", STR_LANGNAME, ": multiplicative arithmetic expression parsed");
}

	/*
 ************************************************************
 * multiplicative arithmetic expression Prime
 * BNF: <multiplicative arithmetic expression Prime> -> <maePrimePrime> <maePrime> | epsilon
 * FIRST(<maePrime>) = { INL_T, FL_T, ID_T, LPR_T, ARI_T }
 ***********************************************************
 */
	chopin_void maePrime() {
		psData.parsHistogram[BNF_maePrime]++;
		switch (lookahead.code) {
		case ARI_T:
			switch (lookahead.attribute.codeType) {
			case OP_MUL:
			case OP_DIV:
				maePrimePrime();
				maePrime();
				break;
			default:
				;
			}
		default:
			;
		}
		printf("%s%s\n", STR_LANGNAME, ": multiplicative arithmetic expression Prime parsed");
	}
/*
 ************************************************************
 * multiplicative arithmetic expression PrimePrime
 * BNF: <maePrimePrime> -> ARI_T(*) <maePrime> | ARI_T(/) <maePrime>
 * FIRST(<maePrimePrime>) = { ARI_T }
 ***********************************************************
 */
chopin_void maePrimePrime() {
	psData.parsHistogram[BNF_maePrimePrime]++;
	switch (lookahead.code) {
	case ARI_T:
		if (lookahead.attribute.codeType == OP_MUL) {
			matchToken(ARI_T, NO_ATTR);
			primaryArithmeticExpression();
		}
		else if (lookahead.attribute.codeType == OP_DIV) {
			matchToken(ARI_T, NO_ATTR);
			primaryArithmeticExpression();
		}
		break;
	default:
		printf("maePP\n");
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": multiplicative arithmetic expression PrimePrime parsed");
}
/*
 ************************************************************
 * primary arithmetic expression
 * BNF: <primary arithmetic expression> -> ID_T | INL_T | FL_T | LPR_T <additive arithmetic expression> RPR_T
 * FIRST(<output statement>) = { INL_T, FL_T, ID_T, <float type variable>, LPR_T }
 ***********************************************************
 */
chopin_void primaryArithmeticExpression() {
	psData.parsHistogram[BNF_primaryArithmeticExpression]++;
	switch (lookahead.code) {
	case ID_T:
		matchToken(ID_T, NO_ATTR);
		break;
	case INL_T:
		matchToken(INL_T, NO_ATTR);
		break;
	case FL_T:
		matchToken(FL_T, NO_ATTR);
		break;
	case LPR_T:
		matchToken(LPR_T, NO_ATTR);
		additiveArithmeticExpression();
		matchToken(RPR_T, NO_ATTR);
		break;
	default:
		printf("inside primary ae\n");
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": primary arithmetic expression parsed");
}

/*
 ************************************************************
 * selection statement
 * BNF: <selection expression> -> KEY_T(if) LPR_T <logicalOrExpression> RPR_T
 						LBR_T <opt_statements> RBR_T
   						<optional else statement>

 * FIRST(<selection statement>) = { KW_if }
 ***********************************************************
 */
chopin_void selectionStatement() {
	psData.parsHistogram[BNF_selectionStatement]++;
	matchToken(KW_T, KW_if);
	matchToken(LPR_T, NO_ATTR);
	logicalOrExpression();
	matchToken(RPR_T, NO_ATTR);
	matchToken(LBR_T, NO_ATTR);
	optStatements();
	matchToken(RBR_T, NO_ATTR);
	optionalElseStatement();
	
	printf("%s%s\n", STR_LANGNAME, ": selection statement parsed");
}

/*
 ************************************************************
 * optinoal else statement
 * BNF: <optional else statement> ->  KEY_T(else) { <opt_statements> } | ϵ

 * FIRST(<selection statement>) = { KW_else }
 ***********************************************************
 */
chopin_void optionalElseStatement() {
	psData.parsHistogram[BNF_optionalElseStatement]++;
	if (lookahead.code == KW_T && lookahead.attribute.codeType == KW_else) {
		matchToken(KW_T, KW_else);
		matchToken(LBR_T, NO_ATTR);
		optStatements();
		matchToken(RBR_T, NO_ATTR);
	}

	printf("%s%s\n", STR_LANGNAME, ": optional else statement parsed");
}
/*
************************************************************
*logical OR expression
* BNF: <logical  OR expression> ->  logicalAndExpression logicalOrStatementPrime


* FIRST(<selection statement>) = { LOG_T, ID_T, INL_T, FL_T }
***********************************************************
*/
chopin_void logicalOrExpression() {
	psData.parsHistogram[BNF_logicalOrExpression]++;
	logicalAndExpression();
	logicalOrExpressionPrime();	
	printf("%s%s\n", STR_LANGNAME, ": logical OR expression parsed");
}
/*
************************************************************
*logical OR statement Prime
* BNF: <logical  OR expressionPrime> -> LOG_T(||) <logical AND expression> <logical Or Expression Prime> | epsilon


* FIRST(<selection statement>) = { LOG_T }
***********************************************************
*/
chopin_void logicalOrExpressionPrime() {
	psData.parsHistogram[BNF_logicalOrExpressionPrime]++;
	switch (lookahead.code) {
	case LOG_T:
		if (lookahead.attribute.codeType == OP_OR) {
			matchToken(LOG_T, NO_ATTR);
			logicalAndExpression();
			logicalOrExpressionPrime();
		}
		break;
	default:
		; //empty
	}
	printf("%s%s\n", STR_LANGNAME, ": logical OR expression Prime parsed");
}
/*
************************************************************
*logical And expression
* BNF: <logical  And expression> ->  logicalNotExpression logicalAndStatementPrime


* FIRST(<selection statement>) = { LOG_T, ID_T, INL_T, FL_T }
***********************************************************
*/
chopin_void logicalAndExpression() {
	psData.parsHistogram[BNF_logicalAndExpression]++;
	logicalNotExpression();
	logicalAndExpressionPrime();
	printf("%s%s\n", STR_LANGNAME, ": logical And expression parsed");
}
/*
************************************************************
*logical And statement Prime
* BNF: <logical  And expressionPrime> -> LOG_T(&&) <logical Not expression> <logical AND expression Prime> | epsilon


* FIRST(<selection statement>) = { LOG_T }
***********************************************************
*/
chopin_void logicalAndExpressionPrime() {
	psData.parsHistogram[BNF_logicalAndExpressionPrime]++;
	switch (lookahead.code) {
	case LOG_T:
		if (lookahead.attribute.codeType == OP_AND) {
			matchToken(LOG_T, NO_ATTR);
			logicalNotExpression();
			logicalAndExpressionPrime();
		}
		break;
	default:
		; //empty
	}
	printf("%s%s\n", STR_LANGNAME, ": logical And expression Prime parsed");
}

/*
************************************************************
*logical Not expression
* BNF: <logical NOT expression> ->  LOG_T(!) <relational expression> 		
    					| <relational expression>

* FIRST(<selection statement>) = { LOG_T, ID_T, INL_T, FL_T }
***********************************************************
*/
chopin_void logicalNotExpression() {
	psData.parsHistogram[BNF_logicalNotExpression]++;
	switch (lookahead.code) {
	case LOG_T:
		if (lookahead.attribute.codeType == OP_NOT) {
			matchToken(LOG_T, NO_ATTR);
			relationalExpression();
		}
		break;
	default:
		relationalExpression();
	}
	printf("%s%s\n", STR_LANGNAME, ": logical Not expression parsed");
}
/*
************************************************************
*relational expression
* BNF: <relational expression> ->  <primary relational expression> <relational expression Prime>

* FIRST(<selection statement>) = { LOG_T, ID_T, INL_T, FL_T }
***********************************************************
*/
chopin_void relationalExpression() {
	psData.parsHistogram[BNF_relationalExpression]++;
	switch (lookahead.code) {
	case INL_T:
		matchToken(INL_T, NO_ATTR);
		break;
	case FL_T:
		matchToken(FL_T, NO_ATTR);
		break;
	case ID_T:
		matchToken(ID_T, NO_ATTR);
		break;
	default:
		printf("relational expression");
		printError();
	}
	relationalExpressionPrime();
	printf("%s%s\n", STR_LANGNAME, ": relational expression parsed");
}
/*
************************************************************
*relational expression Prime
* BNF: <relational expression Prime> ->  REL_T(<) <primary relational expression>
*											| REL_T(>) <primary relational expression>
*											| REL_T(<) <primary relational expression>
*											| REL_T(>=) <primary relational expression>
*											| REL_T(<=) <primary relational expression>
*											| REL_T(==) <primary relational expression>
*											| REL_T(!=) <primary relational expression>

* FIRST(<relationals expression Prime>) = { LOG_T, ID_T, INL_T, FL_T }
***********************************************************
*/
chopin_void relationalExpressionPrime() {
	psData.parsHistogram[BNF_relationalExpressionPrime]++;
	switch (lookahead.code) {
	case REL_T:
		switch (lookahead.attribute.codeType) {
		case OP_GT:
		case OP_GTE:
		case OP_LT:
		case OP_LTE:
		case OP_EQ:
		case OP_NE:
			matchToken(REL_T, NO_ATTR);
			if (lookahead.code == INL_T) {
				matchToken(INL_T, NO_ATTR);
			}
			else if (lookahead.code == FL_T) {
				matchToken(FL_T, NO_ATTR);
			}
			else if (lookahead.code == ID_T) {
				matchToken(ID_T, NO_ATTR);
			}
			break;
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": relational expression Prime parsed");
}
/*
************************************************************
* interation statement
* BNF: <iteration statement> -> 	KW_T(while) LPR_T <logical OR expression> RPR_T LBR_T <optstatements> RBR_T
					 				| KW_T(do) LBR_T <optstatements> RBR_T KEY_T(while) LPR_T <logical OR expression> RPR_T 
* FIRST(<iteration statement>) = { KW_T(do), KW_T(while) }
***********************************************************
*/
chopin_void iterationStatement() {
	psData.parsHistogram[BNF_iterationStatement]++;
	if (lookahead.attribute.codeType == KW_while) {
		matchToken(KW_T, KW_while);
		matchToken(LPR_T, NO_ATTR);
		logicalOrExpression();
		matchToken(RPR_T, NO_ATTR);
		matchToken(LBR_T, NO_ATTR);
		optStatements();
		matchToken(RBR_T, NO_ATTR);
	}
	else if (lookahead.attribute.codeType == KW_do) {
		matchToken(KW_T, KW_do);
		matchToken(LBR_T, NO_ATTR);
		optStatements();
		matchToken(RBR_T, NO_ATTR);
		matchToken(KW_T, KW_while);
		matchToken(LPR_T, NO_ATTR);
		logicalOrExpression();
		matchToken(RPR_T, NO_ATTR);
	}
	else {
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": iteration statement parsed");
}

/*
************************************************************
* input statement
* BNF: 


* FIRST(<input statement>) = { ID_T(read) }
***********************************************************
*/
chopin_void inputStatement() {
	psData.parsHistogram[BNF_inputStatement]++;
	matchToken(ID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	inputStatementPrime();
	matchToken(RPR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": input statement parsed");
}
/*
************************************************************
* input statement Prime
* BNF: <input statement Prime> ->   <variable list> | STR_T


* FIRST(<input statement>) = { ID_T(read) }
***********************************************************
*/
chopin_void inputStatementPrime() {
	psData.parsHistogram[BNF_inputStatementPrime]++;
	if (lookahead.code == STR_T) {
		matchToken(STR_T, NO_ATTR);
	}
	else {
		variableList();
	}
	printf("%s%s\n", STR_LANGNAME, ": input statement Prime parsed");
}
/*
************************************************************
* output statement
* BNF:


* FIRST(<output statement>) = { ID_T(write) }
***********************************************************
*/
chopin_void outputStatement() {
	psData.parsHistogram[BNF_outputStatement]++;
	matchToken(ID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	inputStatementPrime();
	matchToken(RPR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": output statement parsed");
}
/*
************************************************************
* output statement Prime
* BNF: <output statement Prime> ->   <variable list> | STR_T


* FIRST(<output statement>) = { ID_T(write) }
***********************************************************
*/
chopin_void outputStatementPrime() {
	psData.parsHistogram[BNF_outputStatementPrime]++;
	if (lookahead.code == STR_T) {
		matchToken(STR_T, NO_ATTR);
	}
	else {
		variableList();
	}
	printf("%s%s\n", STR_LANGNAME, ": output statement Prime parsed");
}
/*
************************************************************
* variable list
* BNF:


* FIRST()
***********************************************************
*/
chopin_void variableList() {
	psData.parsHistogram[BNF_variableList]++;
	matchToken(ID_T, NO_ATTR);
	variableListPrime();
	printf("%s%s\n", STR_LANGNAME, ": variable list parsed");
}
/*
************************************************************
* variable list Prime
* BNF:


* FIRST()
***********************************************************
*/
chopin_void variableListPrime() {
	psData.parsHistogram[BNF_variableListPrime]++;
	if (lookahead.code == COM_T) {
		matchToken(COM_T, NO_ATTR);
		matchToken(ID_T, NO_ATTR);
		variableListPrime();
	}
	printf("%s%s\n", STR_LANGNAME, ": variable list Prime parsed");
}
/*
************************************************************
* note Declaration
* BNF:


* FIRST( KW_note )
***********************************************************
*/
chopin_void noteDeclaration() {
	psData.parsHistogram[BNF_noteDeclaration]++;
		matchToken(KW_T, KW_Note);
		matchToken(ID_T, NO_ATTR);
		optNoteAssignment();
	printf("%s%s\n", STR_LANGNAME, ": note declaration parsed");
}

/*
************************************************************
* optional note assignment
* BNF:

* FIRST( EQU_T )
***********************************************************
*/
chopin_void optNoteAssignment() {
	psData.parsHistogram[BNF_optNoteAssignment]++;
	switch(lookahead.code) {
	case EQU_T:
		matchToken(EQU_T, NO_ATTR);
		noteExpression();
		break;
	default:
		;
}
printf("%s%s\n", STR_LANGNAME, ": optional note assignment parsed");
}

/*
************************************************************
* Note expression
* BNF:

* FIRST( LBR_T )
***********************************************************
*/
chopin_void noteExpression() {
	psData.parsHistogram[BNF_noteExpression]++;
	matchToken(LBR_T, NO_ATTR);
	noteAttributions();
	matchToken(RBR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": noteExpression parsed");
}
/*
************************************************************
* Note attributions
* BNF:

* FIRST( FL_T )
***********************************************************
*/
chopin_void noteAttributions() {
	psData.parsHistogram[BNF_noteAttributions]++;
	if (lookahead.code == FL_T) {
		matchToken(FL_T, NO_ATTR);
	}
	else if (lookahead.code == INL_T) {
		matchToken(INL_T, NO_ATTR);
	}
	else if (lookahead.code == ID_T) {
		matchToken(ID_T, NO_ATTR);
	}
	matchToken(COM_T, NO_ATTR);
	if (lookahead.code == FL_T) {
		matchToken(FL_T, NO_ATTR);
	}
	else if (lookahead.code == INL_T) {
		matchToken(INL_T, NO_ATTR);
	}
	else if (lookahead.code == ID_T) {
		matchToken(ID_T, NO_ATTR);
	}
	matchToken(COM_T, NO_ATTR);
	if (lookahead.code == FL_T) {
		matchToken(FL_T, NO_ATTR);
	}
	else if (lookahead.code == INL_T) {
		matchToken(INL_T, NO_ATTR);
	}
	else if (lookahead.code == ID_T) {
		matchToken(ID_T, NO_ATTR);
	}
	matchToken(COM_T, NO_ATTR);
	if (lookahead.code == FL_T) {
		matchToken(FL_T, NO_ATTR);
	}
	else if (lookahead.code == INL_T) {
		matchToken(INL_T, NO_ATTR);
	}
	else if (lookahead.code == ID_T) {
		matchToken(ID_T, NO_ATTR);
	}
	printf("%s%s\n", STR_LANGNAME, ": noteAttributions parsed");
}

/*
************************************************************
* Track Declaration
* BNF:


* FIRST( KW_track )
***********************************************************
*/
chopin_void trackDeclaration() {
	psData.parsHistogram[BNF_trackDeclaration]++;
	matchToken(KW_T, KW_Track);
	matchToken(ID_T, NO_ATTR);
	optTrackAssignment();
	printf("%s%s\n", STR_LANGNAME, ": Track declaration parsed");
}
/*
************************************************************
* optional Track assignment
* BNF:

* FIRST( EQU_T )
***********************************************************
*/
chopin_void optTrackAssignment() {
	psData.parsHistogram[BNF_optTrackAssignment]++;
	switch (lookahead.code) {
	case EQU_T:
		matchToken(EQU_T, NO_ATTR);
		trackExpression();
		break;
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": optional Track assignment parsed");
}
/*
************************************************************
* Track expression
* BNF:

* FIRST( LBR_T )
***********************************************************
*/
chopin_void trackExpression() {
	psData.parsHistogram[BNF_trackExpression]++;
	matchToken(LBR_T, NO_ATTR);
	trackAttributions();
	matchToken(RBR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Track Expression parsed");
}
/*
************************************************************
* Track attributions
* BNF:

* FIRST( FL_T )
***********************************************************
*/
chopin_void trackAttributions() {
	psData.parsHistogram[BNF_trackAttributions]++;
	if (lookahead.code == STR_T) {
		matchToken(STR_T, NO_ATTR);
	}
	else if (lookahead.code == ID_T) {
		matchToken(ID_T, NO_ATTR);
	}
	matchToken(COM_T, NO_ATTR);
	matchToken(LBR_T, NO_ATTR);
	optNoteVariableList();
	matchToken(RBR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Track Attributions parsed");
}


/*
************************************************************
* optional Note variable list
* BNF:


* FIRST( ID_T )
***********************************************************
*/
chopin_void optNoteVariableList() {
	psData.parsHistogram[BNF_optNoteVariableList]++;
	if (lookahead.code == ID_T) {
		matchToken(ID_T, NO_ATTR);
		noteVariableListPrime();
	}
	printf("%s%s\n", STR_LANGNAME, ": Note variable list parsed");
}
/*
************************************************************
* Note variable list Prime
* BNF:


* FIRST( COM_T )
***********************************************************
*/
chopin_void noteVariableListPrime() {
	psData.parsHistogram[BNF_noteVariableListPrime]++;
	if (lookahead.code == COM_T) {
		matchToken(COM_T, NO_ATTR);
		matchToken(ID_T, NO_ATTR);
		noteVariableListPrime();
	}
	printf("%s%s\n", STR_LANGNAME, ": Note variable list Prime parsed");
}

/*
************************************************************
* project Declaration
* BNF:


* FIRST( KW_project )
***********************************************************
*/
chopin_void projectDeclaration() {
	psData.parsHistogram[BNF_projectDeclaration]++;
	matchToken(KW_T, KW_Project);
	matchToken(ID_T, NO_ATTR);
	optProjectAssignment();
	printf("%s%s\n", STR_LANGNAME, ": project declaration parsed");
}
/*
************************************************************
* optional project assignment
* BNF:

* FIRST( EQU_T )
***********************************************************
*/
chopin_void optProjectAssignment() {
	psData.parsHistogram[BNF_optProjectAssignment]++;
	switch (lookahead.code) {
	case EQU_T:
		matchToken(EQU_T, NO_ATTR);
		projectExpression();
		break;
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": optional project assignment parsed");
}
/*
************************************************************
* project expression
* BNF:

* FIRST( LBR_T )
***********************************************************
*/
chopin_void projectExpression() {
	psData.parsHistogram[BNF_projectExpression]++;
	matchToken(LBR_T, NO_ATTR);
	projectAttributions();
	matchToken(RBR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": project Expression parsed");
}
/*
************************************************************
* project attributions
* BNF:

* FIRST( FL_T )
***********************************************************
*/
chopin_void projectAttributions() {
	psData.parsHistogram[BNF_projectAttributions]++;
	int code;
	if (lookahead.code == STR_T) {
		matchToken(STR_T, NO_ATTR);
	}
	else if (lookahead.code == ID_T) {
		matchToken(ID_T, NO_ATTR);
	}
	matchToken(COM_T, NO_ATTR);
	if (lookahead.code == STR_T) {
		matchToken(STR_T, NO_ATTR);
	}
	else if (lookahead.code == ID_T) {
		matchToken(ID_T, NO_ATTR);
	}
	matchToken(COM_T, NO_ATTR);
	code = lookahead.code;
	if (code == INL_T || code == FL_T || code == ID_T) {
		matchToken(code, NO_ATTR);
	}
	matchToken(COM_T, NO_ATTR);
	matchToken(LBR_T, NO_ATTR);
	optTrackVariableList();
	matchToken(RBR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": project Attributions parsed");
}


/*
************************************************************
* optional track variable list
* BNF:


* FIRST( ID_T )
***********************************************************
*/
chopin_void optTrackVariableList() {
	psData.parsHistogram[BNF_optTrackVariableList]++;
	if (lookahead.code == ID_T) {
		matchToken(ID_T, NO_ATTR);
		trackVariableListPrime();
	}
	printf("%s%s\n", STR_LANGNAME, ": optional track variable list parsed");
}
/*
************************************************************
* track variable list Prime
* BNF:


* FIRST( COM_T )
***********************************************************
*/
chopin_void trackVariableListPrime() {
	psData.parsHistogram[BNF_trackVariableListPrime]++;
	if (lookahead.code == COM_T) {
		matchToken(COM_T, NO_ATTR);
		matchToken(ID_T, NO_ATTR);
		trackVariableListPrime();
	}
	printf("%s%s\n", STR_LANGNAME, ": track variable list Prime parsed");
}

/*
************************************************************
* sharp statement
* BNF: <sharp statement> -> sharp LPR_T ID_T RPR_T


* FIRST( "sharp" )
***********************************************************
*/
chopin_void sharpStatement() {
	// the sharp statement takes two parameters, a string for the type of intonation and a pitch
	psData.parsHistogram[BNF_sharpStatement]++;
	matchToken(ID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	matchToken(STR_T, NO_ATTR);
	matchToken(COM_T, NO_ATTR);
	matchToken(ID_T, NO_ATTR);
	matchToken(RPR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": sharp statement parsed");
}

/*
************************************************************
* flat statement
* BNF: <flat statement> -> flat LPR_T ID_T RPR_T


* FIRST( "flat" )
***********************************************************
*/
chopin_void flatStatement() {
	// the flat function takes two parameters, a string for the type of intonation, and a pitch
	psData.parsHistogram[BNF_flatStatement]++;
	matchToken(ID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	matchToken(STR_T, NO_ATTR);
	matchToken(COM_T, NO_ATTR);
	matchToken(ID_T, NO_ATTR);
	matchToken(RPR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": flat statement parsed");
}

/*
 ************************************************************
 * The function prints statistics of BNF rules
 * Param:
 *	- Parser data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
/*
chopin_void printBNFData(ParserData psData) {
}
*/
chopin_void printBNFData(ParserData psData) {
	/* Print Parser statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_BNF_RULES; cont++) {
		if (psData.parsHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", BNFStrTable[cont], "]=", psData.parsHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}
