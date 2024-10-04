#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include "stack.h"
#include "check.h"

void stack_ctor (Stack_t *stk, int capacity, StackElem_t poison_value)
{
    assert(stk);

    stk->dump_file          = fopen("dump_file.log", "w");
    stk->memory_report_file = fopen("memory_file.log", "w");

    stk->data = (StackElem_t*) calloc (capacity + 2, sizeof(StackElem_t));

    if (stk->data)
    {
    stk->left_canary  = (Canary_t*)stk->data;

    stk->data         = stk->data + 1;
    stk->capacity     = capacity;
    stk->size         = 0;
    stk->hash_current = hash(stk);
    stk->poison       = poison_value;

    stk->right_canary = (Canary_t*) (stk->data + capacity);
    }
    else
    {
        fputs("Memory allocation\n", stk->dump_file);
        assert(0);
    }

    *stk->left_canary  = (StackElem_t) 3565;
    *stk->right_canary = (StackElem_t) 3802;

    for ( size_t i = 0; i < stk->capacity; i++)
        stk->data[i] = stk->poison;

}


void dtor (Stack_t *stk)
{
    assert(stk);

    free(stk->data);

    if (fclose (stk->dump_file) || fclose (stk->memory_report_file))
    {
        fputs ("File hasn't closed\n", stk->dump_file);
        assert(0);
    }

}

void change_stack_capacity (Stack_t *stk, Change_t change)
{
    stack_assert_func(stk);

    memory_report (stk, BEFORE_CHANGE);


    if (change == SHRINK)
        stk->capacity = stk->capacity / 2;

    else if (change == INCREASE)
        stk->capacity = stk->capacity * 2;

    else
    {
        STACK_DUMP(stk);
        assert(0);
    }

    StackElem_t *new_stack_pointer = (StackElem_t*) realloc (stk->data - 1, (stk->capacity + 2) * sizeof(StackElem_t));

    if (!new_stack_pointer)
    {
        fprintf (stk->dump_file, "Memory Allocation\n");
        STACK_DUMP(stk);
        dtor(stk);
        assert(0);
    }

    stk->data = new_stack_pointer;

    stk->left_canary = (Canary_t*)stk->data;
    stk->data += 1;
    stk->right_canary = (Canary_t*) (stk->data + stk->capacity);

    *stk->left_canary  = (StackElem_t) DED;
    *stk->right_canary = (StackElem_t) EDA;

    for (size_t i = stk->size ; i < stk->capacity; i++)
        stk->data[i] = stk->poison;

    memory_report (stk, AFTER_CHANGE);
}

void stack_pop (Stack_t *stk, StackElem_t *x)
{
    stack_assert_func(stk);

    if (stk->size >= 1)
    {
        if (stk->size <= stk->capacity / 4)
            change_stack_capacity (stk, SHRINK);

        stk->size--;
        *x = stk->data[stk->size];
    }

    stk->hash_current = hash(stk);
}

void stack_push (Stack_t *stk, StackElem_t elem)
{
    stack_assert_func(stk);

    if (stk->size == stk->capacity)
    {
        change_stack_capacity (stk, INCREASE);
    }

    stk->data[stk->size] = elem;
    stk->size++;

    stk->hash_current = hash(stk);
}

unsigned long hash (Stack_t *stk)
{
    assert(stk);

    size_t hash_val = 5381;
    for (size_t i = 0; i < stk->size * sizeof(stk->data[0]); i++)
    {
        char c = *((char *)stk->data + i);
        hash_val = ((hash_val << 5) + hash_val) + c;

    }
    return hash_val;
}

bool check_hash (Stack_t *stk)
{
    if (hash(stk) != stk->hash_current)
    {
        STACK_DUMP(stk);
        fputs ("!!!Values have been damaged!!!", stk->dump_file);
        return false;
    }
    return true;
}
