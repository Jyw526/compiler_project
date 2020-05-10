#pragma once
/* glob.h - global variable for the project
 * programmer 백지수, 송주은, 정연우
 * date 05/07/2020
 */

enum errorTypes { ILLICHAR, ILLIDENT };
typedef enum errorTypes ERRORtypes;

int nextid;
extern yylex();
extern int cErrors;
extern int line;
extern int st_idx;
extern char* yytext;
extern int yyleng;
extern void reportError(ERRORtypes err);
extern char* yytext;
extern int SymbolTable();
