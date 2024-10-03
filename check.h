#ifndef CHECK
#define CHECK

void stack_dump (Stack_t *stk, int line, const char* file);
void stack_assert_func(Stack_t *stk);
void memory_report (Stack_t *stk, Status_of_change status);
Error_of_stack stack_error (Stack_t *stk);

#endif
