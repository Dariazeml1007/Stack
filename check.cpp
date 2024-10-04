#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>

#include "stack.h"
#include "check.h"

void memory_report (Stack_t *stk, Status_of_change status)
{
    stack_assert_func(stk);

    switch (status)
    {
        case BEFORE_CHANGE :
        {

            fprintf (stk->memory_report_file, "=========================================\n"
                                              "old address : 0x%p\n"
                                              "old capacity : %d\n",
                                              stk->data,
                                              stk->capacity);
            break;
        }
    case AFTER_CHANGE :
        {
            fprintf (stk->memory_report_file, "=========================================\n"
                                              "new address : 0x%p\n"
                                              "new capacity : %d\n",
                                              stk->data,
                                              stk->capacity);
            break;
        }
    case FREE_:
        {
            fprintf (stk->memory_report_file, "=========================================\n"
                                              "FREE\n"
                                              "address : 0x%p\n",
                                              stk);
            break;
        }
    default:
        {
            printf("Not correct status");
            assert(0);
        }
    }
}

void stack_assert_func(Stack_t *stk)
{
    if (stack_error(stk) == STK_ERROR)
    {
        printf ("!!!NULL pointer on stk!!!\n");
        assert(0);
    }

    if (stack_error(stk) == DATA_ERROR)
    {
        STACK_DUMP(stk);
        dtor(stk);
        assert(0);
    }

    if (!check_hash(stk))
    {
        fputs("Values were damaged", stk->dump_file);
        dtor(stk);
        assert(0);
    }

    if (*stk->left_canary != DED || *stk->right_canary != EDA)
    {
        fputs("Values were damaged", stk->dump_file);
        dtor(stk);
        assert(0);
    }

}

void stack_dump (Stack_t *stk, int line, const char* file)
{
    fprintf (stk->dump_file, "=========================================\n"
                             "file : %s\n"
                             "line : %d\n"
                             "stack_structure : 0x%p\n"
                             "capacity : %u\n"
                             "size : %u\n"
                             "hash : %lu\n",
                             file,
                             line,
                             stk,
                             stk->capacity,
                             stk->size,
                             stk->hash_current);


    if (stk->data)
    {
        fprintf (stk->dump_file, "stack : 0x%p\nleft_canary : 0x%p value : %d\n", stk->data, stk->left_canary, *stk->left_canary);
        for (size_t i = 0; i < stk -> capacity; i++)
        {
            if (i < stk->size)
                fprintf (stk->dump_file, "*data[%u] = %f  address = 0x%p\n", i, stk->data[i], stk->data + i);
            else
                fprintf (stk->dump_file, "data[%u] = %f address = 0x%p\n", i, stk->data[i], stk->data + i);
        }
        fprintf (stk->dump_file, "right_canary : 0x%p value : %d\n", stk->right_canary, *stk->right_canary);

    }
}

Error_of_stack stack_error(Stack_t *stk)
{
    if (!stk)
    {
        return STK_ERROR;
    }

    if (!stk->data)
    {
       return DATA_ERROR;
    }

    return OK_STACK;
}
