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

  char** argv = malloc(6 * sizeof(char*));

  argv[0] = "--option";
  argv[1] = "value";
  argv[2] = "--next-option";
  argv[3] = "--third-option";
  argv[4] = "-k";
  argv[5] = "short-value";

  saneopt_t* opt = saneopt_init(6, argv);
  assert(strcmp(saneopt_get(opt, "option"), "value") == 0);
  assert(strcmp(saneopt_get(opt, "next-option"), "") == 0);
  assert(strcmp(saneopt_get(opt, "third-option"), "") == 0);
  assert(strcmp(saneopt_get(opt, "k"), "short-value") == 0);

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

void test_short_alias() {
  printf("test_short_alias()\n");

  char** argv = malloc(2 * sizeof(char*));

  argv[0] = "-o";
  argv[1] = "value";

  saneopt_t* opt = saneopt_init(2, argv);
  saneopt_alias(opt, "option", "o");
  assert(strcmp(saneopt_get(opt, "option"), "value") == 0);

  free(argv);
  free(opt);
}

void test_stop_after_arguments() {
  printf("test_stop_after_arguments()\n");

  char** argv = malloc(5 * sizeof(char*));

  argv[0] = "--option";
  argv[1] = "value";
  argv[2] = "--";
  argv[3] = "--next-option";
  argv[4] = "value";

  saneopt_t* opt = saneopt_init(5, argv);
  assert(strcmp(saneopt_get(opt, "option"), "value") == 0);
  assert(saneopt_get(opt, "next-option") == NULL);

  free(argv);
  free(opt);
}

void test_arguments() {
  printf("test_arguments()\n");

  char** argv = malloc(6 * sizeof(char*));
  char** args = NULL;
  int argc;

  argv[0] = "--option";
  argv[1] = "value";
  argv[2] = "arg0";
  argv[3] = "arg1";
  argv[4] = "--";
  argv[5] = "arg2";

  saneopt_t* opt = saneopt_init(6, argv);
  argc = saneopt_arguments(opt, &args);
  assert(strcmp(saneopt_get(opt, "option"), "value") == 0);
  assert(argc == 3);
  assert(strcmp(args[0], "arg0") == 0);
  assert(strcmp(args[1], "arg1") == 0);
  assert(strcmp(args[2], "arg2") == 0);

  free(argv);
  free(opt);
}

int main(int argc, char** argv) {
  test_no_arg();
  test_no_value();
  test_value();
  test_long_alias();
  test_short_alias();
  test_stop_after_arguments();
  test_arguments();

  return 0;
}
