enum errorTypes { ILLICHAR, ILLIDENT };
typedef enum errorTypes ERRORtypes;

extern void reportError(ERRORtypes err);
extern char* yytext;