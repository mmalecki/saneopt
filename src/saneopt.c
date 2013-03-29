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

char* saneopt_get(saneopt_t* opt, char* name) {
  int i;
  char* arg;

  for (i = 0; i < opt->argc; i++) {
    arg = opt->argv[i];
    if (strncmp(arg, "--", 2) == 0) {
      if (strcmp(arg + 2, name) == 0) {
        return ((i + 1) < opt->argc && opt->argv[i + 1][0] != '-')
          ? opt->argv[i + 1]
          : "";
      }
    }
  }

  return NULL;
}
