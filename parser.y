%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>
#include "glob.h"

/*yacc source for Mini C*/
extern void ReportError();
extern yylex();
extern yyerror(char *s);
%}
%token TIDENT TNUMBER TCONST TELSE TIF TINT TRETURN TVOID TWHILE TRNUMBER TFLOAT
%token TADDASSIGN TSUBASSIGN TMULASSIGN TDIVASSIGN TMODASSIGN
%token TOR TAND TEQUAL TNOTEQU TGREAT TLESS TGREATE TLESSE TINC TDEC
%token TPLUS TMINUS TSTAR TSLASH TMOD TASSIGN TNOT TCOMMA TSEMI TBROPEN TBRCLOSE
%token TCURLOPEN TCURLCLOSE TSQUOPEN TSQUCLOSE
%nonassoc LOWER_THAN_ELSE
%nonassoc TELSE
%%
mini_c 				: translation_unit
                    ;
translation_unit 	: external_dcl
					| translation_unit external_dcl
                    ;
external_dcl 		: function_def
		  			| declaration
                    | TIDENT TSEMI
                    | TIDENT error
                    {
                        yyerrok;
                        printError(wrong_st); /* error - wrong statement */
                    }
                    ;
function_def 		: function_header compound_st
                    | function_header TSEMI
                    | function_header error /* 비정상적인 함수 정의 */
                    {
                        /* 에러발생시 type 수정을 위해 default값 '0' 세팅 */
                        /* identifier about parse error */
                        look_tmp->type=0;
                        /*
                        look_tmp=look_id;
                        look_tmp->itype=notype;
                        look_tmp->vtype=noval;
                        */
                        yyerrok;
                        /* error - wrong function definition */
                        printError(wrong_funcdef);
                        /*
                        ReportError(nosemi);
                        */
                    }
                    ;
function_header 	: dcl_spec function_name formal_param
                    | dcl_spec formal_param
                    {
                        yyerrok;
                        ReportError(wrong_funcdef);
                    }
                    ;
dcl_spec 			: dcl_specifiers
                    ;
dcl_specifiers 		: dcl_specifier
				 	| dcl_specifiers dcl_specifier
                    ;
dcl_specifier 		: type_qualifier
					| type_specifier
                    ;
type_qualifier 		: TCONST
                    ;
type_specifier 		: TINT  { type_int=1;} /* type:integer */
		 			| TVOID {type_void=1;}
                    /*
                    | TINT { look_tmp->itype = tint; }
		 			| TVOID { look_tmp->itype = tvoid; }
		 			| TFLOAT { look_tmp->itype = tfloat; }
		 			| TCHAR { look_tmp->itype = tchar; }
                    */
                    ;
function_name 		: TIDENT
                    {
                        /* identifier about parse error or not defined identifier/function */
                        if(look_id->type==0 || look_id->type==5){
                            look_id->type = 4;  /*function name*/
                            type_int=0; /*not integer*/
                            type_void=0;    /*not void*/
                            look_tmp=look_id;
                        }
                        /*
                        if( look_id->vtype==noval || look_id->itype==notype ){
							look_id->vtype = tfunc;
							look_id->itype = look_tmp->itype;
							//look_tmp->vtype = look_id->vtype;
							//look_tmp->itype = look_id->itype;
						}
                        */
                    }
                    ;
formal_param 		: TBROPEN opt_formal_param TBRCLOSE
                    ;
opt_formal_param 	: formal_param_list
		   			|
                    ;
formal_param_list 	: param_dcl
		    		| formal_param_list TCOMMA param_dcl
		    		| formal_param_list param_dcl
		    		{
						yyerrok;
						ReportError(wrong_param);
					}
                    ;
param_dcl 			: dcl_spec declarator
                    ;
compound_st 		: TCURLOPEN opt_dcl_list opt_stat_list TCURLCLOSE
                    | TCURLOPEN opt_dcl_list opt_stat_list error
                    {
                        yyerrok;
                        printError(nobrace);
                    }
                    ;
opt_dcl_list 		: declaration_list
					|
                    ;
declaration_list 	: declaration
					| declaration_list declaration
                    ;
declaration 		: dcl_spec init_dcl_list TSEMI
                    {
                        type_int=0; /*not integer*/
                        type_void=0; /*not void*/
                        /*
                        look_tmp->itype = notype;
						look_tmp->vtype = noval;
                        */
                    }
                    | dcl_spec init_dcl_list error
                    {
                        look_tmp->type=0; /*identifier about parse error*/
                        yyerrok;
                        type_int=0;
                        type_void=0;
                        printError(nosemi);
                        /*
                        yyerrok;
						look_tmp = look_id;
						ReportError(nosemi);
						look_tmp->itype = notype;
						look_tmp->vtype = noval;
                        */
                    }
                    ;
init_dcl_list 		: init_declarator
					| init_dcl_list TCOMMA init_declarator
                    ;
init_declarator 	: declarator
		 			| declarator TASSIGN TNUMBER
		 			| declarator TASSIGN TRNUMBER
                    ;
declarator 			: TIDENT
                    {
                        if(look_id->type==0){ /* 현재 identifier가 type field를 가리키면*/
                            if(type_int==1) /* type:integer */
                                look_id->type=1;    /* integer scalar variable */
                            else if(type_void==1)
                                look_id->type=2;
                        }
                        look_tmp=look_id;
                        /*
                        if( look_id->itype==notype ){
							look_id->vtype=tscalar;
							
							if( look_tmp->itype==tint )
								look_id->itype = tint;
							else if( look_tmp->itype==tvoid )
								look_id->itype = tvoid;
							else if( look_tmp->itype==tfloat )
								look_id->itype = tfloat;
							else if( look_tmp->itype==tchar )
								look_id->itype = tchar;						
						}
						//look_tmp = look_id;
                        */
                    }
	     			| TIDENT TSQUOPEN opt_number TSQUCLOSE
                    {
                        if(look_id->type==0){   /* 현재 identifier가 type field를 가리키면 */
                            look_id->type=3;    /* array integer variable */
                            look_tmp=look_id;
                        }
                        /*
                        if( look_id->itype==notype ){
	     					look_id->vtype = tarray;

	     					if( look_tmp->itype==tint )
								look_id->itype = tint;
							else if( look_tmp->itype==tvoid )
								look_id->itype = tvoid;
							else if( look_tmp->itype==tfloat )
								look_id->itype = tfloat;
							else if( look_tmp->itype==tchar )
								look_id->itype = tchar;
	     				}
	     				//look_tmp = look_id;
                        */
                    }
                    | TIDENT TSQUOPEN opt_number error
                    {
                        yyerrok;
                        printError(nobracket);
                    }
                    ;
opt_number 			: TNUMBER
                    | TRNUMBER
	     			|
                    ;
opt_stat_list 		: statement_list
		 			|
                    ;
statement_list 		: statement
		 			| statement_list statement
                    | statement_list declaration
                    ;
statement 			: compound_st
	   				| expression_st
	   				| if_st
	   				| while_st
	   				| return_st
	   				;
expression_st 		: opt_expression TSEMI
                    | expression error
                    {
                        yyerrok;
                        printError(nosemi);
                    }
                    ;
opt_expression 		: expression
		 			|
                    ;
if_st 				: TIF TBROPEN expression TBRCLOSE statement %prec LOWER_THAN_ELSE
					| TIF TBROPEN expression TBRCLOSE statement TELSE statement
                    ;
while_st 			: TWHILE TBROPEN expression TBRCLOSE statement
                    ;
return_st 			: TRETURN opt_expression TSEMI
                    ;
expression 			: assignment_exp
                    ;
assignment_exp 		: logical_or_exp
					| unary_exp TASSIGN assignment_exp
					| unary_exp TADDASSIGN assignment_exp
					| unary_exp TSUBASSIGN assignment_exp
					| unary_exp TMULASSIGN assignment_exp
					| unary_exp TDIVASSIGN assignment_exp
					| unary_exp TMODASSIGN assignment_exp
					;
logical_or_exp 		: logical_and_exp
					| logical_or_exp TOR logical_and_exp
                    ;
logical_and_exp 	: equality_exp
		 			| logical_and_exp TAND equality_exp
                    ;
equality_exp 		: relational_exp
					| equality_exp TEQUAL relational_exp
					| equality_exp TNOTEQU relational_exp
                    ;
relational_exp 		: additive_exp
					| relational_exp TGREAT additive_exp
					| relational_exp TLESS additive_exp
					| relational_exp TGREATE additive_exp
					| relational_exp TLESSE additive_exp
                    ;
additive_exp 		: multiplicative_exp
					| additive_exp TPLUS multiplicative_exp
					| additive_exp TMINUS multiplicative_exp
                    ;
multiplicative_exp 	: unary_exp
		 		   	| multiplicative_exp TSTAR unary_exp
		  		  	| multiplicative_exp TSLASH unary_exp
			    	| multiplicative_exp TMOD unary_exp
                    ;
unary_exp 			: postfix_exp
	   				| TMINUS unary_exp
	   				| TNOT unary_exp
	   				| TINC unary_exp
	   				| TDEC unary_exp
                    ;
postfix_exp 		: primary_exp
	     	 		| postfix_exp TSQUOPEN expression TSQUCLOSE
                    | postfix_exp TSQUOPEN expression error
	      			{
	      				yyerrok;
	      				ReportError(nobracket);
	      			}
	      			| postfix_exp TBROPEN opt_actual_param TBRCLOSE
	     	 		| postfix_exp TINC
	      			| postfix_exp TDEC
                    ;
opt_actual_param 	: actual_param
				  	|
                    ;
actual_param 		: actual_param_list
                    ;
actual_param_list 	: assignment_exp
		   			| actual_param_list TCOMMA assignment_exp
                    ;
primary_exp 		: TIDENT
                    {
                        if(look_id->type==0) /* 현재 identifier가 type field를 가리키면 */
                            look_id->type=5; /* not defined identifier/function */
                        /*
                        if( look_id->vtype==noval )
							look_id->itype = notdef;
                        */
                    }
	     			| TNUMBER
                    | TRNUMBER
	     			| TBROPEN expression TBRCLOSE
                    ;
%%