#include <stdlib.h>
#include <string.h>

#include "../include/saneopt.h"

saneopt_t* saneopt_init(int argc, char** argv) {
  saneopt_t* saneopt = malloc(sizeof(saneopt_t));
  saneopt->argc = argc;
  saneopt->argv = argv;

  saneopt->aliases = NULL;
  saneopt->alias_count = 0;
  return saneopt;
}

int saneopt__matches(saneopt_t* opt, char* option, char* arg) {
  int i;

  if (strncmp(arg, "--", 2) == 0) {
    arg += 2;

    if (strcmp(arg, option) == 0) {
      return 1;
    }

    for (i = 0; opt->aliases && i < opt->alias_count; i++) {
      if (strcmp(opt->aliases[i]->option, option) == 0 &&
          strlen(opt->aliases[i]->alias) > 1 &&
          strcmp(arg, opt->aliases[i]->alias) == 0) {
        return 1;
      }
    }
  }
  else if (strncmp(arg, "-", 1) == 0) {
    ++arg;

    if (strcmp(arg, option) == 0) {
      return 1;
    }

    for (i = 0; opt->aliases && i < opt->alias_count; i++) {
      if (strcmp(opt->aliases[i]->option, option) == 0 &&
          strlen(opt->aliases[i]->alias) == 1 &&
          strcmp(arg, opt->aliases[i]->alias) == 0) {
        return 1;
      }
    }
  }

  return 0;
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

char* saneopt_get(saneopt_t* opt, char* option) {
  int i;
  char* arg;

  for (i = 0; i < opt->argc; i++) {
    arg = opt->argv[i];

    if (strcmp(arg, "--") == 0)
      return NULL;

    if (saneopt__matches(opt, option, arg)) {
      return ((i + 1) < opt->argc && opt->argv[i + 1][0] != '-')
        ? opt->argv[i + 1]
        : "";
    }
  }

  return NULL;
}
