#pragma once
/* glob.h - global variable for the project
 * programmer 백지수, 송주은, 정연우
 * date 05/07/2020
 */
 
enum errorTypes { ILLICHAR, ILLIDENT };
typedef enum errorTypes ERRORtypes;

int cErrors = 0;
int line = 1; //token line number
int st_idx = -1; //st index (default=-1)

extern yylex();
extern int yyleng;
extern void reportError(ERRORtypes err);
extern char* yytext;
extern int SymbolTable();
