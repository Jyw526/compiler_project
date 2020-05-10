/* main.c - 각 token에 대한 출력
 * programmer - 백지수,송주은,정연우
 * date - 05/04/2020
 */

#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
#include "glob.h"

void printToken(enum token tt);

void main(){
    enum token tt;
    //print header
    printf("Line number\tToken type\tST-index\tToken\n");
    printf("----------------------------------------------------------\n");

    //call yylex()
    while((tt=yylex()) != TEOF) {
        printf("\t%d\t",line);
       	printToken(tt);
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
void printToken(enum token tt){
    switch (tt){
		case TCONST: printf("TCONST\t"); break;
        case TELSE: printf("TELSE\t"); break;
		case TIF : printf("TIF\t"); break;
        case TINT : printf("TINT\t"); break;
        case TRETURN : printf("TRETURN\t"); break;
        case TVOID : printf("TVOID\t"); break;
        case TWHILE : printf("TWHIL\tE"); break;

        case TPLUS : printf("TPLUS\t"); break;
        case TMINUS : printf("TMINUS\t"); break;
        case TSTAR : printf("TSTAR\t"); break;
        case TSLASH : printf("TSLASH\t"); break;
        case TMOD : printf("TMOD\t"); break;

        case TASSIGN : printf("TASSIGN\t"); break;
        case TADDASSIGN : printf("TADDASSIGN"); break;
        case TSUBASSIGN :printf("TSUBASSIGN"); break;
        case TDIVASSIGN : printf("TDIVASSIGN"); break;
        case TMODASSIGN : printf("TMODASSIGN"); break;

        case TNOT : printf("TNOT\t"); break;
        case TAND : printf("TAND\t"); break;
        case TOR : printf("TOR\t"); break;

        case TEQUAL : printf("TEQUAL\t"); break;
        case TNOTEQU : printf("TNOTEQU\t"); break;
        case TLESS : printf("TLESS\t"); break;
        case TGREAT : printf("TGREAT\t"); break;
        case TLESSE : printf("TLESSE\t"); break;
        case TGREATE : printf("TGREATE\t"); break;
        
        case TINC : printf("TINC\t"); break;
        case TDEC : printf("TDEC\t"); break;

        case TBROPEN : printf("TBROPEN\t"); break;
        case TBRCLOSE : printf("TBRCLOSE"); break;
        case TCOMMA : printf("TCOMMA\t"); break;
        case TCURLOPEN : printf("TCURLOPEN"); break;
        case TCURLCLOSE : printf("TCURLCLOSE"); break;
        case TSQUOPEN : printf("TSQUOPEN"); break;
        case TSQUCLOSE : printf("TSQUCLOSE"); break;
        case TSEMI : printf("TSEMI\t"); break;

        case TIDENT :printf("TIDENT"); break; 
        case TNUMBER : printf("TNUMBER\t"); break;
        case TRNUMBER: printf("TRNUMBER"); break;

        //에러
        case TILLICHAR: reportError(ILLICHAR); break;
        case TILLIDENT: reportError(ILLIDENT); break;

        }
        if(tt == TIDENT){ //identifier인 경우 st-index 출력
            printf("\t\t%d\t\t%s\n",st_idx,yytext);
        }
        else if(tt != TILLICHAR && tt!= TILLIDENT){
            printf("\t\t\t%s\n",yytext);
        }
}