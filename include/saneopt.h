#ifndef _SANEOPT_H
#define _SANEOPT_H

struct saneopt__alias {
  char* option;
  char* alias;
} typedef saneopt__alias_t;

struct saneopt {
  int argc;
  char** argv;

  saneopt__alias_t** aliases;
  unsigned int alias_count;
} typedef saneopt_t;

/*
 * Create a new `saneopt` parser.
 */
saneopt_t* saneopt_init(int argc, char** argv);

/*
 * Set an alias from `option` to `alias`.
 */
void saneopt_alias(saneopt_t* opt, char* option, char* alias);

/*
 * Get option called `option`.
 */
char* saneopt_get(saneopt_t* opt, char* option);

/*
 * Get command line arguments, that is: any arguments not being an argument
 * value and all arguments after "--".
 *
 * For example, parsing:
 *
 *   ./app --option value foo bar -- --not-option baz
 *
 * Will result in this function returning ["foo", "bar", "--not-option", "baz"].
 */
char** saneopt_arguments(saneopt_t* opt);

#endif
