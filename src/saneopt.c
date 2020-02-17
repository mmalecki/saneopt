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

void saneopt_free(saneopt_t* opt) {
  int i;
  if (opt->aliases) {
    for (i = 0; opt->aliases && i < opt->alias_count; i++) {
      free(opt->aliases[i]);
    }
    free(opt->aliases);
  }
  free(opt);
}

int saneopt__matches(saneopt_t* opt, const char* option, const char* arg) {
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

int saneopt_alias(saneopt_t* opt, const char* option, const char* alias) {
  saneopt__alias_t** aliases;
  saneopt__alias_t* alias_ = malloc(sizeof(saneopt__alias_t));

  alias_->option = option;
  alias_->alias = alias;

  aliases = realloc(
    opt->aliases,
    ++opt->alias_count * sizeof(saneopt__alias_t*)
  );

  if (aliases == NULL)
    return -1;

  opt->aliases = aliases;
  opt->aliases[opt->alias_count - 1] = alias_;

  return 0;
}

char* saneopt_get(saneopt_t* opt, const char* option) {
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

char** saneopt_get_all(saneopt_t* opt, const char* option) {
  int i;
  int count = 0;
  char* arg;
  char** result = malloc(sizeof(char*));
  result[0] = NULL;

  for (i = 0; i < opt->argc; i++) {
    arg = opt->argv[i];

    if (strcmp(arg, "--") == 0)
      return result;

    if (saneopt__matches(opt, option, arg)) {
      result = realloc(result, sizeof(char*) * (++count + 1));

      if (result == NULL) {
        free(result);
        return NULL;
      }

      result[count - 1] = ((i + 1) < opt->argc && opt->argv[i + 1][0] != '-')
        ? opt->argv[i + 1]
        : "";
      result[count] = NULL;
    }
  }

  return result;
}

char** saneopt_arguments(saneopt_t* opt) {
  int i, j, count = 0, saw_option = 0, saw_value = 0;
  char* arg;
  char** result = NULL;

  for (i = 0; i < opt->argc; i++) {
    arg = opt->argv[i];

    if (strcmp(arg, "--") == 0) {
      //
      // When we find the marker, copy rest of the command line to the result
      // and return.
      //
      result = realloc(result, sizeof(char*) * (count + opt->argc - i));
      if (result == NULL)
        return NULL;

      for (j = i + 1; j < opt->argc; j++)
        result[count + j - i - 1] = opt->argv[j];

      result[count + opt->argc - i - 1] = NULL;
      return result;
    }

    if (strncmp(arg, "-", 1) == 0) {
      saw_option = 1;
      continue;
    }
    else if (saw_option) {
      saw_option = 0;
      saw_value = 1;
      continue;
    }

    if (saw_value || !saw_option) {
      result = realloc(result, sizeof(char*) * (++count + 1));

      if (result == NULL)
        return NULL;

      result[count - 1] = arg;
      result[count] = NULL;
      saw_value = 0;
    }
  }

  return result;
}
