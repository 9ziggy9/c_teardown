#include <stdio.h>
#include <string.h>

#define TEARDOWN_IMPL
#include "teardown.h"

void teardown_string(int status, void *resource) {
  (void) status;
  char **p_str = (char **) resource;
  if (*p_str == NULL) fprintf(stdout, "String previously freed: %s\n", *p_str);
  else {
    fprintf(stderr, "Freeing string: %s\n", *p_str);
    free(*p_str);
    *p_str = NULL;
  }
}

int main(void) {
  TeardownStack *td_stack = teardown_stack_create();
  on_exit(exec_teardown, td_stack);

  char *msg1 = strdup("First dynamically allocated string.");
  register_teardown_unit(td_stack, &msg1, teardown_string);

  char *msg2 = strdup("Second dynamically allocated string.");
  register_teardown_unit(td_stack, &msg2, teardown_string);

  exit(EXIT_SUCCESS);
}
