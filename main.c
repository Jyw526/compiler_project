/*
 * main.c - 각 token에 대한 출력
 * programmer - 백지수,송주은,정연우
 * date - 05/04/2020
 */

#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
extern yylex();
extern char *yytext;

int cErrors =0;

void main(){
    enum token tt;
    //print header
    printf("Line number\tToken type\tST-index\tToken\n");
    printf("---------------------------------------------------\n");

    //call yylex()
    while((tt=yylex()) != EOF) {
       	printtoken(tt);
	}

    //report error
    if(cErrors == 0)
        printf("No errors detected\n");
    else
        printf("%d errors deteced\n",cErrors);
    
}

/*
*printtoken() - token type 별로 출력
*/
void printtoken(enum token tt){
    switch (tt){
		case TCONST: printf("TCONST"); break;
        case TELSE: printf("TELSE"); break;
		case TIF : printf("TIF"); break;
        case TINT : printf("TINT"); break;
        case TRETURN : printf("TRETURN"); break;
        case TVOID : printf("TVOID"); break;
        case TWHILE : printf("TWHILE"); break;

        case TPLUS : printf("TPLUS"); break;
        case TMINUS : printf("TMINUS"); break;
        case TSTAR : printf("TSTAR"); break;
        case TSLASH : printf("TSLASH"); break;
        case TMOD : printf("TMOD"); break;

        case TASSIGN : printf("TASSIGN"); break;
        case TADDASSIGN : printf("TADDASSIGN"); break;
        case TSUBASSIGN :printf("TSUBASSIGN"); break;
        case TDIVASSIGN : printf("TDIVASSIGN"); break;
        case TMODASSIGN : printf("TMODASSIGN"); break;

        case TNOT : printf("TNOT"); break;
        case TAND : printf("TAND"); break;
        case TOR : printf("TOR"); break;

        case TEQUAL : printf("TEQUAL"); break;
        case TNOTEQU : printf("TNOTEQU"); break;
        case TLESS : printf("TLESS"); break;
        case TGREAT : printf("TGREAT"); break;
        case TLESSE : printf("TLESSE"); break;
        case TGREATE : printf("TGREATE"); break;
        
        case TINC : printf("TINC"); break;
        case TDEC : printf("TDEC"); break;

        case TBROPEN : printf("TBROPEN"); break;
        case TBRCLOSE : printf("TBRCLOSE"); break;
        case TCOMMA : printf("TCOMMA"); break;
        case TCURLOPEN : printf("TCURLOPEN"); break;
        case TCURLCLOSE : printf("TCURLCLOSE"); break;
        case TSQUOPEN : printf("TSQUOPEN"); break;
        case TSQUCLOSE : printf("TSQUCLOSE"); break;
        case TSEMI : printf("TSEMI"); break;

        case TIDENT :printf("TIDENT"); break;
        case TNUMBER : printf("TNUMBER"); break;
        case TRNUMBER : printf("TRNUMBER");
        }
        printf("\t\t%s\n",yytext);
}