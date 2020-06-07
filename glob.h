#pragma once
/* glob.h - global variable for the project
 * programmer 백지수, 송주은, 정연우
 * date 06/04/2020
 */

#define STsize 1000	//size of string table
#define HTsize 100	//size of hash table

enum errorTypes { ILLICHAR, ILLIDENT, wrong_st, wrong_param, wrong_funcdef, wrong_def, nosemi, nobracket, nobrace, nosqubracket, nocomma, nocondition}; //parse error 추가해야함
typedef enum errorTypes ERRORtypes;
//identifier type
enum id_type { parse_error, void_function, int_function, float_function, void_scalar, int_scalar, float_scalar, void_array, int_array, float_array };

typedef struct HTentry* HTpointer;
typedef struct HTentry {
	int index;		//index of identifier in ST
	int line_num;		//line number of identifier in input string
	enum id_type type;       //type of identifier
	HTpointer next;	//pointer to next identifier
} HTentry;

HTpointer cur_ID;

int nextid;
extern yylex();
extern yyerrok();
extern int cErrors;
extern int line;
extern int st_idx;
extern char* yytext;
extern int yyleng;
extern void reportError(ERRORtypes err);
extern char* yytext;
extern int SymbolTable();
extern void PrintHStable();