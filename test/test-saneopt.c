#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <saneopt.h>

void test_no_arg() {
  printf("test_no_arg()\n");

  char** argv;

  saneopt_t* opt = saneopt_init(0, argv);
  assert(saneopt_get(opt, "no-option") == NULL);

  free(opt);
}

void test_no_value() {
  printf("test_no_value()\n");

  char** argv = malloc(1 * sizeof(char*));

  argv[0] = "--option";

  saneopt_t* opt = saneopt_init(1, argv);
  assert(strcmp(saneopt_get(opt, "option"), "") == 0);

  free(argv);
  free(opt);
}

void test_value() {
  printf("test_value()\n");

  char** argv = malloc(4 * sizeof(char*));

  argv[0] = "--option";
  argv[1] = "value";
  argv[2] = "--next-option";
  argv[3] = "--third-option";

  saneopt_t* opt = saneopt_init(4, argv);
  assert(strcmp(saneopt_get(opt, "option"), "value") == 0);
  assert(strcmp(saneopt_get(opt, "next-option"), "") == 0);
  assert(strcmp(saneopt_get(opt, "third-option"), "") == 0);

  free(argv);
  free(opt);
}

void test_long_alias() {
  printf("test_long_alias()\n");

  char** argv = malloc(3 * sizeof(char*));

  argv[0] = "--some-opt";
  argv[1] = "--alias";
  argv[2] = "value";

  saneopt_t* opt = saneopt_init(3, argv);
  saneopt_alias(opt, "option", "alias");
  assert(strcmp(saneopt_get(opt, "option"), "value") == 0);

  free(argv);
  free(opt);
}

int main(int argc, char** argv) {
  test_no_arg();
  test_no_value();
  test_value();
  test_long_alias();

  return 0;
}
