#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
#include "glob.h"
extern cErrors;
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