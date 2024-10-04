#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "check.h"

int main()
{
    Stack_t stk = {};

    StackElem_t POISON = 666.13;
    stack_ctor (&stk, 10, POISON);
    stack_push (&stk, 11);
    STACK_DUMP(&stk);



    for (int i = 0; i < 100; i++)
        stack_push (&stk, i);
    STACK_DUMP(&stk);

    StackElem_t x = 0;
    for (int i = 0; i < 70; i++)
        stack_pop (&stk, &x);



    STACK_DUMP(&stk);

    memory_report(&stk, FREE_);

    dtor(&stk);
    return 0;
}

