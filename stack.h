#ifndef STACK
#define STACK

typedef double StackElem_t;
const double POISON_VALUE = 10000.f;
#define output_id "%f"

#define STACK_DUMP(stk_ptr) stack_dump (stk_ptr, __LINE__, __FILE__);

struct Stack_t
{
    StackElem_t *data = {};
    size_t size = 0;
    size_t capacity = 0;
    FILE *dump_file = NULL;
    FILE *memory_report_file = NULL;
    unsigned long hash_current = 0;
    StackElem_t poison = 0;
};

typedef enum
{
    SHRINK,
    INCREASE
} Change_t;

typedef enum
{
    STK_ERROR = 1,
    DATA_ERROR = 2,
    OK_STACK = 0
} Error_of_stack;

typedef enum
{
    BEFORE_CHANGE = -1,
    FREE_ = 0,
    AFTER_CHANGE = 1

} Status_of_change;

void stack_ctor (Stack_t *stk, int capacity, StackElem_t poison_value);
void dtor(Stack_t *stk);

void stack_push (Stack_t *stk, StackElem_t elem);
void stack_pop (Stack_t *stk, StackElem_t *x);

void change_stack_capacity (Stack_t *stk, Change_t coefficient);
unsigned long hash (Stack_t *stk);
bool check_hash (Stack_t *stk);

#endif
