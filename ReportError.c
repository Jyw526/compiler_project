/* ReportError.c - 각 error에 대한 출력
 * programmer - 백지수,송주은,정연우
 * date - 05/04/2020
 */

#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
#include "glob.h"

void reportError(ERRORtypes err) {
	switch (err) {
	case ILLICHAR :
		printf("**Error**\t\t\t%s Illegal Character\n",yytext);
		cErrors++;
		break;
	case ILLIDENT :
		printf("**Error**\t\t\t%s Illegal IDENT\n",yytext);
		cErrors++;
		break;
	}
	
}