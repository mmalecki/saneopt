#include <stdlib.h>
#include <string.h>

#include "../include/saneopt.h"

saneopt_t* saneopt_new(int argc, char** argv) {
  saneopt_t* saneopt = malloc(sizeof(saneopt_t));
  saneopt->argc = argc;
  saneopt->argv = argv;

  saneopt->aliases = NULL;
  saneopt->alias_count = 0;
  return saneopt;
}

void saneopt_alias(saneopt_t* opt, char* option, char* alias) {
  saneopt__alias_t* alias_ = malloc(sizeof(saneopt__alias_t));
  alias_->option = option;
  alias_->alias = alias;

  opt->aliases = realloc(
    opt->aliases,
    ++opt->alias_count * sizeof(saneopt__alias_t*)
  );
  opt->aliases[opt->alias_count - 1] = alias_;
}

int saneopt_empty(saneopt_arg_t* arg) {
  return arg == NULL || arg->value == NULL;
}

saneopt_arg_t* saneopt_get(saneopt_t* opt, char* name) {
  int i, argc;
  char* arg;
  char** argv;
  saneopt_arg_t* result;

  argc = opt->argc;
  argv = opt->argv;

  for (i = 0; i < argc; i++) {
    arg = argv[i];
    if (strncmp(arg, "--", 2) == 0) {
      if (strcmp(arg + 2, name) == 0) {
        result = malloc(sizeof(saneopt_arg_t));
        if ((i + 1) < argc && argv[i + 1][0] != '-') {
          result->value = argv[i + 1];
          return result;
        }
      }
    }
  }

  return NULL;
}
