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

  saneopt_free(opt);
}

void test_no_value() {
  printf("test_no_value()\n");

  char** argv = malloc(1 * sizeof(char*));

  argv[0] = "--option";

  saneopt_t* opt = saneopt_init(1, argv);
  assert(strcmp(saneopt_get(opt, "option"), "") == 0);

  free(argv);
  saneopt_free(opt);
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
  saneopt_free(opt);
}

void test_long_alias() {
  printf("test_long_alias()\n");

  char** argv = malloc(3 * sizeof(char*));

  argv[0] = "--some-opt";
  argv[1] = "--alias";
  argv[2] = "value";

  saneopt_t* opt = saneopt_init(3, argv);
  assert(saneopt_alias(opt, "option", "alias") == 0);
  assert(strcmp(saneopt_get(opt, "option"), "value") == 0);

  free(argv);
  saneopt_free(opt);
}

void test_short_alias() {
  printf("test_short_alias()\n");

  char** argv = malloc(2 * sizeof(char*));

  argv[0] = "-o";
  argv[1] = "value";

  saneopt_t* opt = saneopt_init(2, argv);
  assert(saneopt_alias(opt, "option", "o") == 0);
  assert(strcmp(saneopt_get(opt, "option"), "value") == 0);

  free(argv);
  saneopt_free(opt);
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
  saneopt_free(opt);
}

void test_arguments() {
  printf("test_arguments()\n");

  char** argv = malloc(6 * sizeof(char*));
  char** args;

  argv[0] = "--option";
  argv[1] = "value";
  argv[2] = "arg0";
  argv[3] = "arg1";
  argv[4] = "--";
  argv[5] = "arg2";

  saneopt_t* opt = saneopt_init(6, argv);
  args = saneopt_arguments(opt);
  assert(strcmp(saneopt_get(opt, "option"), "value") == 0);
  assert(strcmp(args[0], "arg0") == 0);
  assert(strcmp(args[1], "arg1") == 0);
  assert(strcmp(args[2], "arg2") == 0);
  assert(args[3] == NULL);

  free(argv);
  saneopt_free(opt);
}

void test_arguments_first() {
  printf("test_arguments_first()\n");

  char** argv = malloc(2 * sizeof(char*));
  char** args;

  argv[0] = "value";
  argv[1] = "next-value";

  saneopt_t* opt = saneopt_init(2, argv);
  args = saneopt_arguments(opt);
  assert(strcmp(args[0], "value") == 0);
  assert(strcmp(args[1], "next-value") == 0);
  assert(args[2] == NULL);

  free(argv);
  saneopt_free(opt);
}

void test_arguments_first_marker() {
  printf("test_arguments_first_marker()\n");

  char** argv = malloc(2 * sizeof(char*));
  char** args;

  argv[0] = "--";
  argv[1] = "value";
  argv[2] = "next-value";

  saneopt_t* opt = saneopt_init(3, argv);
  args = saneopt_arguments(opt);
  assert(strcmp(args[0], "value") == 0);
  assert(strcmp(args[1], "next-value") == 0);
  assert(args[2] == NULL);

  free(argv);
  saneopt_free(opt);
}

void test_arguments_many_markers() {
  printf("test_arguments_many_markers()\n");

  char** argv = malloc(9 * sizeof(char*));
  char** args;

  argv[0] = "-o";
  argv[1] = "stdout.log";
  argv[2] = "--";
  argv[3] = "estragon";
  argv[4] = "-h";
  argv[5] = "127.0.0.1:1337";
  argv[6] = "--";
  argv[7] = "node";
  argv[8] = "script.js";

  saneopt_t* opt = saneopt_init(9, argv);
  args = saneopt_arguments(opt);
  assert(strcmp(args[0], "estragon") == 0);
  assert(strcmp(args[1], "-h") == 0);
  assert(strcmp(args[2], "127.0.0.1:1337") == 0);
  assert(strcmp(args[3], "--") == 0);
  assert(strcmp(args[4], "node") == 0);
  assert(strcmp(args[5], "script.js") == 0);
  assert(args[6] == NULL);

  free(argv);
  saneopt_free(opt);
}

// https://github.com/mmalecki/saneopt/issues/2
void test_arguments_issue_2() {
  printf("test_arguments_issue_2()\n");

  char** argv = malloc(7 * sizeof(char*));
  char** args;

  argv[0] = "-h";
  argv[1] = "127.0.0.1:1337";
  argv[2] = "-h";
  argv[3] = "127.0.0.1:1338";
  argv[4] = "--";
  argv[5] = "node";
  argv[6] = "test/fixtures/listen.js";

  saneopt_t* opt = saneopt_init(7, argv);
  args = saneopt_arguments(opt);
  assert(strcmp(args[0], "node") == 0);
  assert(strcmp(args[1], "test/fixtures/listen.js") == 0);
  assert(args[2] == NULL);

  free(argv);
  saneopt_free(opt);
}

// https://github.com/mmalecki/saneopt/issues/3
void test_arguments_issue_3() {
  printf("test_arguments_issue_3()\n");

  char** argv = malloc(8 * sizeof(char*));
  char** args;

  argv[0] = "-o";
  argv[1] = "out.log";
  argv[2] = "--min-uptime";
  argv[3] = "2000";
  argv[4] = "run";
  argv[5] = "--";
  argv[6] = "node";
  argv[7] = "s.js";

  saneopt_t* opt = saneopt_init(8, argv);
  args = saneopt_arguments(opt);
  assert(strcmp(args[0], "run") == 0);
  assert(strcmp(args[1], "node") == 0);
  assert(strcmp(args[2], "s.js") == 0);
  assert(args[3] == NULL);

  free(argv);
  saneopt_free(opt);
}

void test_all() {
  printf("test_all()\n");

  char** argv = malloc(6 * sizeof(char*));
  char** args;

  argv[0] = "--option";
  argv[1] = "first";
  argv[2] = "--option";
  argv[3] = "second";
  argv[4] = "-o";
  argv[5] = "third";

  saneopt_t* opt = saneopt_init(6, argv);
  assert(saneopt_alias(opt, "option", "o") == 0);
  args = saneopt_get_all(opt, "option");
  assert(strcmp(args[0], "first") == 0);
  assert(strcmp(args[1], "second") == 0);
  assert(strcmp(args[2], "third") == 0);
  assert(args[3] == NULL);

  free(argv);
  free(args);
  saneopt_free(opt);
}

int main(int argc, char** argv) {
  test_no_arg();
  test_no_value();
  test_value();
  test_long_alias();
  test_short_alias();
  test_stop_after_arguments();
  test_arguments();
  test_arguments_first();
  test_arguments_first_marker();
  test_arguments_many_markers();
  test_arguments_issue_2();
  test_arguments_issue_3();
  test_all();

  return 0;
}
