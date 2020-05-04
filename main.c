#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
extern yylex();
extern char *yytext;

void main(){
    enum tokentypes tt;
    while((tt=yylex()) != TEOF) {
       	switch (tt){
		case TCONST: printf("TCONST"); break;
        case TELSE: printf("TELSE"); break;
		case TIF : printf("TIF"); break;
        case TINT : printf("TINT"); break;
        case TRETURN : printf("TRETURN"); break;
        case TVOID : printf("TVOID"); break;
        case TWHILE : printf("TWHILE"); break;
        case TEQUAL : printf("TEQUAL"); break;
        case TNOTEQU : printf("TNOTEQU"); break;
        case TLESSE : printf("TLESSE"); break;
        case TGREATE : printf("TGREATE"); break;
        case TAND : printf("TAND"); break;
        case TOR : printf("TOR"); break;
        case TINC : printf("TINC"); break;
        case TDEC : printf("TDEC"); break;
        case TADDASSIGN : printf("TADDASSIGN"); break;
        case TSUBASSIGN :printf("TSUBASSIGN"); break;
        case TDIVASSIGN : printf("TDIVASSIGN"); break;
        case TMODASSIGN : printf("TMODASSIGN"); break;
        case TIDENT :printf("TIDENT"); break;
        case TNUMBER : printf("TNUMBER"); break;
        }
        printf("\t\t\t%s\n",yytext);
	}
}