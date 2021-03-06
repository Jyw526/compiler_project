%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>
#include "glob.h"

/*yacc source for Mini C*/
extern void reportError();
extern yylex();
extern yyerror(char *s);

enum id_type tmp;
int itype=0, vtype=0;
%}
%token TIDENT TNUMBER TCONST TELSE TIF TINT TRETURN TVOID TWHILE TRNUMBER TFLOAT
%token TADDASSIGN TSUBASSIGN TMULASSIGN TDIVASSIGN TMODASSIGN
%token TOR TAND TEQUAL TNOTEQU TGREAT TLESS TGREATE TLESSE TINC TDEC
%token TPLUS TMINUS TSTAR TSLASH TMOD TASSIGN TNOT TCOMMA TSEMI TBROPEN TBRCLOSE
%token TCURLOPEN TCURLCLOSE TSQUOPEN TSQUCLOSE
%nonassoc LOWER_THAN_ELSE
%nonassoc TELSE
%%
mini_c	: translation_unit
	;
translation_unit	: external_dcl
			| translation_unit external_dcl
			;
external_dcl	: function_def
		| declaration
		| TIDENT TSEMI
		| error
		{
			yyerrok;
			reportError(wrong_st); /* error - wrong statement */
		}
		| temp_char { yyerrok; reportError(wrong_st); }
		| temp_close 
		| TSEMI
		;
temp_char	: TADDASSIGN
		| TSUBASSIGN
		| TMULASSIGN
		| TDIVASSIGN
		| TMODASSIGN
		| TNUMBER
		| TRNUMBER
		| TOR
		| TAND
		| TEQUAL
		| TNOTEQU
		| TGREAT
		| TGREATE
		| TLESSE
		| TINC
		| TDEC
		| TPLUS
		| TSTAR
		| TSLASH
		| TMOD
		| TASSIGN
		| TCOMMA
		| TNOT
		| TMINUS
		| TLESS
		;
temp_close	: TBRCLOSE { yyerrok; reportError(nobracket); }
		| TCURLCLOSE { yyerrok; reportError(nobrace); }
		| TSQUCLOSE { yyerrok; reportError(nosqubracket); }
		;
function_def	: function_header compound_st
		| function_header TSEMI
		| function_header error /* ë¹ì ?? ?¸ ?¨? ? ? */
		{
			/* ??¬ë°ì? type ?? ? ??´ defaultê°? '0' ?¸? */
			/* identifier about parse error */
			tmp=parse_error;
			cur_ID->type=tmp;
			itype=0;
			vtype=0;
			yyerrok;
			/* error - wrong function definition */
			reportError(wrong_funcdef);
		}
		;
function_header	: dcl_spec function_name formal_param
		| dcl_spec error formal_param
		{
			yyerrok;
			reportError(wrong_funcdef);
		}
		| function_name formal_param
		{
			yyerrok;
			reportError(wrong_funcdef);
		}
		;
dcl_spec	: dcl_specifiers
		;
dcl_specifiers	: dcl_specifier
		| dcl_specifiers dcl_specifier
		;
dcl_specifier	: type_qualifier
		| type_specifier
		;
type_qualifier	: TCONST
		;
type_specifier	: TINT { itype = 2; }
		| TVOID { itype = 1; }
		| TFLOAT { itype = 3; }
		;
function_name	: TIDENT
		{
			/* identifier about parse error or not defined identifier/function */
			vtype = 0;  /*function name*/
			tmp=vtype+itype;
			cur_ID->type=tmp;
			itype=0;
		}
		;
formal_param	: TBROPEN opt_formal_param TBRCLOSE
		| TBROPEN opt_formal_param error
		{
			yyerrok;
			reportError(nobracket);
		}
		;
opt_formal_param	: formal_param_list
			|
			;
formal_param_list	: param_dcl
			| formal_param_list TCOMMA param_dcl
			| formal_param_list param_dcl
			{
				yyerrok;
				reportError(nocomma);
			}
			;
param_dcl	: dcl_spec declarator
		| dcl_spec error 
		{
			yyerrok;
			reportError(wrong_param);
		}
		| declarator{
			yyerrok;
			reportError(wrong_param);
		}
		;
compound_st	: TCURLOPEN opt_dcl_list opt_stat_list TCURLCLOSE
		| TCURLOPEN opt_dcl_list opt_stat_list error
		{
			yyerrok;
			reportError(nobrace);
		}
		;
opt_dcl_list	: declaration_list
		|
		;
declaration_list	: declaration
			| declaration_list declaration
			;
declaration	: dcl_spec init_dcl_list TSEMI
		{
			itype=0;
			vtype=0;
		}
		| dcl_spec init_dcl_list error
		{
			tmp=parse_error; /*identifier about parse error*/
			yyerrok;
			tmp=itype+vtype;
			cur_ID->type=tmp;
			itype=0;
			vtype=0;
			reportError(nosemi);
		}
		| dcl_spec error TSEMI
		{
			yyerrok;
			reportError(wrong_def);
		}
		;
init_dcl_list	: init_declarator
		| init_dcl_list TCOMMA init_declarator
		| init_dcl_list init_declarator
		{
			yyerrok;
			reportError(nocomma);
		}
		;
init_declarator	: declarator
		| declarator TASSIGN TNUMBER
		| declarator TASSIGN TRNUMBER
		;
declarator	: TIDENT
		{
			vtype=3;
			tmp=itype+vtype;
			cur_ID->type=tmp;
		}
		| TIDENT TSQUOPEN opt_number TSQUCLOSE
		{
			if(cur_ID->type==parse_error){   /* ??¬ identifierê°? type fieldë¥? ê°?ë¦¬í¤ë©? */
				vtype=6;
				tmp=itype+vtype;
				cur_ID->type=tmp;
			}
		}
		| TIDENT TSQUOPEN opt_number error
		{
			yyerrok;
			reportError(nosqubracket);
		}
		;
opt_number	: TNUMBER
		| TRNUMBER
		| TIDENT
		|
		;
opt_stat_list	: statement_list
		|
		;
statement_list	: statement
		| statement_list statement
		| statement_list declaration
		;
statement	: compound_st
		| expression_st
		| if_st
		| while_st
		| return_st
		;
expression_st	: opt_expression TSEMI
		| expression error
		{
			yyerrok;
			reportError(nosemi);
		}
		;
opt_expression	: expression
		|
		;
if_st	: TIF TBROPEN expression TBRCLOSE statement %prec LOWER_THAN_ELSE
	| TIF TBROPEN TBRCLOSE error
	{
		yyerrok;
		reportError(nocondition);
	}
	| TIF TBROPEN expression TBRCLOSE statement TELSE statement
	| TIF TBROPEN expression error
	{
		yyerrok;
		reportError(nobracket);
	}	
	| TIF error
	{
		yyerrok;
		printf("if");
		reportError(nobracket);
	}
	;
while_st	: TWHILE TBROPEN expression TBRCLOSE statement
		| TWHILE TBROPEN TBRCLOSE error
		{
			yyerrok;
			reportError(nocondition);
			
		}
		| TWHILE TBROPEN expression error
		{
			yyerrok;
			reportError(nobracket);
		}
		| TWHILE error
		{
			yyerrok;
			reportError(nobracket);
		}
		;
return_st	: TRETURN opt_expression TSEMI
		| TRETURN opt_expression error
		{
			yyerrok;
			reportError(nosemi);
		}
		;
expression	: assignment_exp
		;
assignment_exp	: logical_or_exp
		| unary_exp TASSIGN assignment_exp
		| unary_exp TASSIGN error
		{
			yyerrok;
			reportError(wrong_st);
		}
		| unary_exp TADDASSIGN assignment_exp
		| unary_exp TADDASSIGN error
		{
			yyerrok;
			reportError(wrong_st);
		}
		| unary_exp TSUBASSIGN assignment_exp
		| unary_exp TSUBASSIGN error
		{
			yyerrok;
			reportError(wrong_st);
		}
		| unary_exp TMULASSIGN assignment_exp
		| unary_exp TMULASSIGN error
		{
			yyerrok;
			reportError(wrong_st);
		}
		| unary_exp TDIVASSIGN assignment_exp
		| unary_exp TDIVASSIGN error
		{
			yyerrok;
			reportError(wrong_st);
		}
		| unary_exp TMODASSIGN assignment_exp
		| unary_exp TMODASSIGN error
		{
			yyerrok;
			reportError(wrong_st);
		}
		| TASSIGN { yyerrok; reportError(wrong_st); }
		| TMULASSIGN { yyerrok; reportError(wrong_st); }
		| TDIVASSIGN { yyerrok; reportError(wrong_st); }
		| TMODASSIGN { yyerrok; reportError(wrong_st); }
		| TSUBASSIGN { yyerrok; reportError(wrong_st); }
		| TADDASSIGN { yyerrok; reportError(wrong_st); }
		;
logical_or_exp	: logical_and_exp
		| logical_or_exp TOR logical_and_exp
		| logical_or_exp TOR error
		{
			yyerrok;
			reportError(wrong_st);
		}
		| TOR { yyerrok; reportError(wrong_st); }
		;
logical_and_exp	: equality_exp
		| logical_and_exp TAND equality_exp
		| logical_and_exp TAND error
		{
			yyerrok;
			reportError(wrong_st);
		}
		| TAND { yyerrok; reportError(wrong_st); }
		;
equality_exp	: relational_exp
		| equality_exp TEQUAL relational_exp
		| equality_exp TEQUAL error
		{
			yyerrok;
			reportError(wrong_st);
		}
		| equality_exp TNOTEQU relational_exp
		| equality_exp TNOTEQU error
		{
			yyerrok;
			reportError(wrong_st);
		}
		| TEQUAL { yyerrok; reportError(wrong_st); }
		| TNOTEQU { yyerrok; reportError(wrong_st); }
		;
relational_exp	: additive_exp
		| relational_exp TGREAT additive_exp
		| relational_exp TGREAT error  { yyerrok; reportError(wrong_st); }
		| relational_exp TLESS additive_exp
		| relational_exp TLESS error  { yyerrok; reportError(wrong_st); }
		| relational_exp TGREATE additive_exp
		| relational_exp TGREATE error  { yyerrok; reportError(wrong_st); }
		| relational_exp TLESSE additive_exp
		| relational_exp TLESSE error  { yyerrok; reportError(wrong_st); }
		| TGREAT  { yyerrok; reportError(wrong_st); }
		| TLESS  { yyerrok; reportError(wrong_st); }
		| TGREATE  { yyerrok; reportError(wrong_st); }
		| TLESSE  { yyerrok; reportError(wrong_st); }
		; 
additive_exp	: multiplicative_exp
		| additive_exp TPLUS multiplicative_exp
		| additive_exp TPLUS error { yyerrok; reportError(wrong_st); }
		| additive_exp TMINUS multiplicative_exp
		| additive_exp TMINUS error { yyerrok; reportError(wrong_st); }
		| TPLUS { yyerrok; reportError(wrong_st); }
		;
multiplicative_exp	: unary_exp
			| multiplicative_exp TSTAR unary_exp
			| multiplicative_exp TSTAR error  { yyerrok; reportError(wrong_st); }
			| multiplicative_exp TSLASH unary_exp
			| multiplicative_exp TSLASH error  { yyerrok; reportError(wrong_st); }
			| multiplicative_exp TMOD unary_exp
			| multiplicative_exp TMOD error  { yyerrok; reportError(wrong_st); }
			;
unary_exp	: postfix_exp
		| TMINUS unary_exp
		| TMINUS error { yyerrok; reportError(wrong_st); }
		| TNOT unary_exp
		| TNOT error { yyerrok; reportError(wrong_st); }
		| TINC unary_exp
		| TINC error { yyerrok; reportError(wrong_st); }
		| TDEC unary_exp
		| TDEC error { yyerrok; reportError(wrong_st); }
		;
postfix_exp	: primary_exp
		| postfix_exp TSQUOPEN expression TSQUCLOSE
		| postfix_exp TSQUOPEN expression error
		{
			yyerrok;
			reportError(nosqubracket);
		}
		| postfix_exp TBROPEN opt_actual_param TBRCLOSE
		| postfix_exp TBROPEN opt_actual_param error
		{
			yyerrok;
			reportError(nobracket);
		}
		| postfix_exp TINC
		| postfix_exp TDEC
		;
opt_actual_param	: actual_param
			|
			;
actual_param	: actual_param_list
		;
actual_param_list	: assignment_exp
			| actual_param_list TCOMMA assignment_exp
			| actual_param_list TCOMMA error { yyerrok; reportError(wrong_st); }
			| TCOMMA { yyerrok; reportError(wrong_st); }
			;
primary_exp	: TIDENT
		| TNUMBER
		| TRNUMBER
		| TBROPEN expression TBRCLOSE
		| TBROPEN expression error
		{
			yyerrok;
			reportError(nobracket);
		}
		;
%%