/* main.c - 각 token에 대한 출력
 * programmer - 백지수,송주은,정연우
 * date - 05/04/2020
 */

#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
#include "glob.h"
extern int yyparse();
int cErrors = 0;

void main() {
    //print header
    yyparse();
    //report error
    if (cErrors == 0)
        printf("No errors detected\n");
    else
        printf("%d errors deteced\n", cErrors);

    PrintHStable();

}
