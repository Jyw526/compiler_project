/* ReportError.c - �� error�� ���� ���
 * programmer - ������,������,������
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