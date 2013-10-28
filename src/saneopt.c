#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <saneopt.h>

char* saneopt__pad_right(char* ptr, char* pad, int length) {
  int i;
  ptr = realloc(ptr, ((strlen(ptr) + length) - strlen(ptr)) * sizeof(char));

  for (i = strlen(ptr); i < length; i++) {
    strcat(ptr, pad);
  }

  return ptr;
}

saneopt_option_t* saneopt_alias(saneopt_option_t* option, char* alias) {
  int i;

  // Immediately return if this alias is already defined for this option.
  for (i = 0; i < option->aliases_length; i++) {
    if (strcmp(option->aliases[i], alias) == 0) {
      return option;
    }
  }

  if (option->aliases == NULL) {
    option->aliases = malloc(sizeof(char*));
  }
  else {
    option->aliases = realloc(option->aliases,
        (option->aliases_length + 1) * sizeof(char*));
  }

  option->aliases[option->aliases_length++] = alias;

  return option;
}

saneopt_t* saneopt_init() {
  saneopt_t* saneopt = malloc(sizeof(saneopt_t));

  saneopt->argv = NULL;
  saneopt->argc = 0;

  saneopt->options = NULL;
  saneopt->options_length = 0;

  saneopt->arguments = NULL;
  saneopt->arguments_length = 0;

  saneopt_option_t* help = saneopt_option(saneopt, "help");
  help->desc = "This help screen.";
  help->requires_value = 0;

  return saneopt;
}

saneopt_option_t* saneopt_get(saneopt_t* saneopt, char* input, int _long) {
  int i, k;
  saneopt_option_t* option;

  for (i = 0; i < saneopt->options_length; i++) {
    option = saneopt->options[i];

    if (_long && strcmp(option->name, input) == 0) {
      return option;
    }
    else if (!_long) {
      for (k = 0; k < option->aliases_length; k++) {
        if (strcmp(option->aliases[k], input) == 0) {
          return option;
        }
      }
    }
  }

  return NULL;
}

void saneopt_help(saneopt_t* saneopt, saneopt_option_t* option) {
  int i, j;
  char* aliases = NULL;
  char* label = NULL;

  if (option != NULL) {
    for (i = 0; i < option->aliases_length; i++) {
      if (aliases == NULL) {
        aliases = malloc((strlen(option->aliases[i])) * sizeof(char));
        sprintf(aliases, "-%s", option->aliases[i]);
      }
      else {
        aliases = realloc(aliases,
          (strlen(aliases) + strlen(option->aliases[i]) + 3) * sizeof(char));
        sprintf(aliases, "%s, -%s", aliases, option->aliases[i]);
      }
    }

    if (option->help) {
      if (option->requires_value) {
        printf("--%s [value]\nAliases: %s\n\n%s\n", option->name, aliases, option->help);
      }
      else {
        printf("--%s\nAliases: %s\n\n%s\n", option->name, aliases, option->help);
      }
    }
    else if (option->desc) {
      if (option->requires_value) {
        printf("--%s [value]\nAliases: %s\n\n%s\n", option->name, aliases, option->desc);
      }
      else {
        printf("--%s\nAliases: %s\n\n%s\n", option->name, aliases, option->desc);
      }
    }
    else {
      printf("No help for %s\n", option->name);
    }

    // Clean up malloc'd aliases
    if (aliases != NULL) {
      free(aliases);
      aliases = NULL;
    }

    return;
  }

  if (saneopt->header != NULL) {
    printf("%s\n\n", saneopt->header);
  }

  for (i = 0; i < saneopt->options_length; i++) {
    option = saneopt->options[i];

    label = malloc(strlen(option->name) * sizeof(char));
    sprintf(label, "%s", option->name);

    for (j = 0; j < option->aliases_length; j++) {
      label = realloc(label, (strlen(label) + strlen(option->aliases[j]) + 4) * sizeof(char));
      sprintf(label, "%s, -%s", label, option->aliases[j]);
    }

    if (option->requires_value) {
      label = realloc(label, (strlen(label) + 9) * sizeof(char));
      sprintf(label, "%s [value]", label);
    }

    label = saneopt__pad_right(label, " ", 30);

    if (option->desc) {
      printf("--%s%s\n", label, option->desc);
    }
    else {
      printf("--%sNo help available\n", label);
    }

    free(label);
    label = NULL;
  }

  // Make sure all malloc'd objects are free
  if (aliases != NULL) free(aliases);
  if (label != NULL) free(label);
}

void saneopt_help_for(saneopt_t* saneopt, char* name) {
  int i, j;
  saneopt_option_t* option;

  for (i = 0; i < saneopt->options_length; i++) {
    option = saneopt->options[i];

    if (strcmp(option->name, name) == 0) {
      saneopt_help(saneopt, option);
      return;
    }

    for (j = 0; j < option->aliases_length; j++) {
      if (strcmp(option->aliases[j], name) == 0) {
        saneopt_help(saneopt, option);
        return;
      }
    }
  }
}

void saneopt_free(saneopt_t* saneopt) {
  int i;
  saneopt_option_t* option;

  for (i = 0; i < saneopt->options_length; i++) {
    option = saneopt->options[i];
    if (option->aliases != NULL) free(option->aliases);
    free(option);
  }

  if (saneopt->options != NULL) free(saneopt->options);
  if (saneopt->arguments != NULL) free(saneopt->arguments);

  free(saneopt);
}

saneopt_option_t* saneopt_option(saneopt_t* saneopt, char* name) {
  int i;

  for (i = 0; i < saneopt->options_length; i++) {
    if (saneopt->options[i]->name == name) {
      return saneopt->options[i];
    }
  }

  if (saneopt->options == NULL) {
    saneopt->options = malloc(sizeof(saneopt_option_t*));
  }
  else {
    saneopt->options = realloc(saneopt->options,
        (saneopt->options_length + 1) * sizeof(saneopt_option_t*));
  }

  saneopt_option_t* option = malloc(sizeof(saneopt_option_t));
  option->name = name;
  option->required = 0;
  option->available = 0;
  option->requires_value = 1;
  option->desc = NULL;
  option->help = NULL;
  option->value = NULL;
  option->aliases = NULL;
  option->aliases_length = 0;

  saneopt->options[saneopt->options_length] = option;

  return saneopt->options[saneopt->options_length++];
}

saneopt_t* saneopt_parse(saneopt_t* saneopt, int argc, char** argv) {
  int i, j;
  char* argument;
  saneopt_option_t* option;

  if (saneopt == NULL) {
    saneopt = saneopt_init();
  }

  if (argc == 1) {
    saneopt_help(saneopt, NULL);
    saneopt_free(saneopt);
    exit(0);
  }

  saneopt->argv = argv;
  saneopt->argc = argc;

  for (i = 1; i < argc; i++) {
    argument = argv[i];

    if (strcmp(argument, "--") == 0) {
      for (j = (i + 1); j < argc; j++) {
        saneopt_push_argument(saneopt, argument);
      }
      break;
    }

    if (strncmp(argument, "--", 2) == 0) {
      argument += 2;

      if (strcmp(argument, "help") == 0) {
        if (i + 1 < argc && strncmp(argv[i + 1], "-", 1) != 0) {
          saneopt_help_for(saneopt, argv[i + 1]);
        }
        else {
          saneopt_help(saneopt, NULL);
        }
        saneopt_free(saneopt);
        exit(0);
      }

      option = saneopt_get(saneopt, argument, 1);
      if (option != NULL) {
        option->available = 1;
        if (option->requires_value && i + 1 < argc &&
            strncmp(argv[i + 1], "-", 1) != 0) {
          option->value = argv[++i];
        }
      }
    }
    else if (strncmp(argument, "-", 1) == 0) {
      argument++;
      option = saneopt_get(saneopt, argument, 0);
      if (option != NULL) {
        option->available = 1;
        if (option->requires_value && i + 1 < argc &&
            strncmp(argv[i + 1], "-", 1) != 0) {
          option->value = argv[++i];
        }
      }
    }
    else {
      saneopt_push_argument(saneopt, argument);
    }
  }

  return saneopt;
}

saneopt_t* saneopt_push_argument(saneopt_t* saneopt, char* argument) {
  if (saneopt->arguments == NULL) {
    saneopt->arguments = malloc(sizeof(char*));
  }
  else {
    saneopt->arguments = realloc(saneopt->arguments,
        (saneopt->arguments_length + 1) * sizeof(char*));
  }

  saneopt->arguments[saneopt->arguments_length++] = argument;

  return saneopt;
}
