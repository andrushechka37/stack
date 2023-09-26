#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"


int main(void) {
    stack stk = {};
    stack_ctor(&stk);
    for(int i = 3; i < 20; i++) {
        stack_push(&stk, i);
        //dump_stk(&stk, " ", 1," ");
    }
    dump_stk(&stk, " ", 1," ");
    for(int i = 15; i > 5; i--) {
        int t = i;
        stack_pop(&stk, &t);
        printf("%d\n", t);
        dump_stk(&stk, " ", 1," ");
    }
    stack_dtor(&stk);
}

