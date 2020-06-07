%{
/* scanner.l - lexical analyzer for the MiniC
 * programmer 백지수, 송주은, 정연우
 * date  05/04/2020
 */
#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
#include "glob.h"

int line=1; //token line number
int st_idx=-1; //st index (default=-1)

int symbolTManagement();
void checkline();
%}
%%
"const"						return(TCONST);
"else"						return(TELSE);
"if"						return(TIF);
"int"						return(TINT);
"float"                     				return(TFLOAT);
"return"					return(TRETURN);
"void"						return(TVOID);
"while"						return(TWHILE);

"+"							return(TPLUS);
"-"							return(TMINUS);
"*"							return(TSTAR);
"/"							return(TSLASH);
"%"							return(TMOD);

"="							return(TASSIGN);
"+="						return(TADDASSIGN);
"-="						return(TSUBASSIGN);
"*="						return(TMULASSIGN);
"/="						return(TDIVASSIGN);
"%="						return(TMODASSIGN);

"!"							return(TNOT);
"&&"						return(TAND);
"||"						return(TOR);

"=="						return(TEQUAL);
"!="						return(TNOTEQU);
"<"							return(TLESS);
">"							return(TGREAT);
"<="						return(TLESSE);
">="						return(TGREATE);

"++"						return(TINC);
"--"						return(TDEC);

"("							return(TBROPEN);
")"							return(TBRCLOSE);
","							return(TCOMMA);
"{"							return(TCURLOPEN);
"}"							return(TCURLCLOSE);
"["							return(TSQUOPEN);
"]"							return(TSQUCLOSE);
";"							return(TSEMI);

[A-Za-z_][A-Za-z0-9_]*		st_idx=symbolTManagement(); return(TIDENT); //symbolTManagement() 
[0-9]|([1-9][0-9]*)					return(TNUMBER);
[0-9]+"."[0-9]+				return(TRNUMBER);

"/*"([^*]|\*+[^*/])*\**"*/" checkline();
"//".*                      checkline();
[ \t]                       ;
"\n"                        line++; 
.			reportError(ILLICHAR);
[0-9]+[A-Za-z_][A-Za-z0-9_]*		reportError(ILLIDENT);
%%
void checkline(){
     for(int i=0;i<yyleng;i++){
        if(yytext[i] == '\n') line++;
    }
}
/*ident matching*/
int symbolTManagement(){
    return SymbolTable();
} 

int yywrap(){
	return 1;
}