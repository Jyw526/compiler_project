/* ReportError.c - 각 error에 대한 출력
 * programmer - 백지수,송주은,정연우
 * date - 05/04/2020
 */

#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
#include "glob.h"

void yyerror(char* s)
{
}
void reportError(ERRORtypes err) {
	printf("Line%d\t\t\t",line);
	cErrors++;
	switch (err) {
	case ILLICHAR:
		printf("Scanner Error: '%s' Illegal Character\n", yytext);
		break;
	case ILLIDENT:
		printf("Scanner Error: '%s' Illegal IDENT\n", yytext);
		break;
	case wrong_st:
		printf("Parser Error: '%s' Wrong statement\n", yytext);
		break;
	case wrong_param:
		printf("Parser Error: Wrong parameter\n");
		break;
	case wrong_funcdef:
		printf("Parser Error: Wrong function definition\n");
		break;
	case nosemi:
		printf("Parser Error: Missing semicolon\n");
		break;
	case nobracket:
		printf("Parser Error: Missing bracket\n");
		break;
	case nobrace:
		printf("Parser Error: Missing brace\n");
		break;
	case nosqubracket:
		printf("Parser Error: Missing square bracket\n");
		break;
	case nocomma:
		printf("Parser Error: Missing comma\n");
		break;
	case wrong_def:
		printf("Parser Error: Wrong variable definition\n");
		break;
	}
}
