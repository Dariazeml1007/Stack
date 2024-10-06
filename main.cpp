#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "check.h"

int main()
{
    Stack_t stk = {};

    StackElem_t POISON = 666.13;
    stack_ctor (&stk, 10, POISON);

    for (int i = 0; i < 30; i++)
        stack_push (&stk, i);

    STACK_DUMP(&stk);


    StackElem_t x = 0;
    stack_pop (&stk, &x);

    printf ("%f", x);


    memory_report(&stk, FREE_);

    dtor(&stk);
    return 0;
}

