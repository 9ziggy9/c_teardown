#ifndef TEARDOWN_H_
#define TEARDOWN_H_
#include <stdlib.h>

typedef void (*teardown_func)(int, void *);
struct teardown_unit {
  struct teardown_unit *next;
  void *resource;
  teardown_func clean;
};
typedef struct { struct teardown_unit *sp; } TeardownStack;
TeardownStack *teardown_stack_create(void);
void exec_teardown(int, void *arg);

#ifdef TEARDOWN_IMPL
TeardownStack *teardown_stack_create(void) {
  return (TeardownStack *) calloc(1, sizeof(TeardownStack));
}

static void
register_teardown_unit(TeardownStack *stk, void *res, teardown_func clean)
{
  struct teardown_unit *tdu = malloc(sizeof(struct teardown_unit));
  tdu->resource = res;
  tdu->clean = clean;
  tdu->next = stk->sp;
  stk->sp = tdu;
}

static struct teardown_unit *
pop_teardown_unit(TeardownStack *stk) {
  if (stk->sp == NULL) return NULL;
  struct teardown_unit *top = stk->sp;
  stk->sp = top->next;
  return top;
}

void exec_teardown(int status, void *arg) {
  TeardownStack *stack = (TeardownStack *) arg;
  struct teardown_unit *curr = NULL;
  while ((curr = pop_teardown_unit(stack)) != NULL) {
    curr->clean(status, curr->resource);
    free(curr);
  }
  free(stack);
}
#endif // TEARDOWN_IMPL
#endif // TEARDOWN_H_
