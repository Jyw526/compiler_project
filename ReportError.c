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
	printf("%s\n", s);
}
void reportError(ERRORtypes err) {
	switch (err) {
	case ILLICHAR:
		printf("%d\t\t\t%s Illegal Character\n", line, yytext);
		cErrors++;
		break;
	case ILLIDENT:
		printf("%d\t\t\t%s Illegal IDENT\n", line, yytext);
		cErrors++;
		break;
	case wrong_st:
		printf("%d\t\t\t Wrong statement\n", line);
		cErrors++;
		break;
	case wrong_param:
		printf("%d\t\t\t Wrong parameter\n", line);
		cErrors++;
		break;
	case wrong_funcdef:
		printf("%d\t\t\t Wrong function definition\n", line);
		cErrors++;
		break;
	case nobrace:
		printf("%d\t\t\t Missing brace\n", line);
		cErrors++;
		break;
	case nosemi:
		printf("%d\t\t\t Missing semicolon\n", line);
		cErrors++;
		break;
	case nobracket:
		printf("%d\t\t\t Missing bracket\n", line);
		cErrors++;
		break;
	}
}