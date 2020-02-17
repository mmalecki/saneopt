#ifndef _SANEOPT_H
#define _SANEOPT_H

#ifdef __cplusplus
extern "C"
{
#endif

struct saneopt__alias {
  const char* option;
  const char* alias;
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
 * Release memory allocated by parser.
 */
void saneopt_free(saneopt_t* opt);

/*
 * Set an alias from `option` to `alias`.
 * Return -1 on error, 0 on success.
 */
int saneopt_alias(saneopt_t* opt, const char* option, const char* alias);

/*
 * Get option called `option`.
 */
char* saneopt_get(saneopt_t* opt, const char* option);

/*
 * Get all values for option called `option`.
 * Return value is a NULL-terminated array.
 *
 * For example, getting all values for "option", with the following args:
 *
 *   ./app --option first --option second
 *
 * Will return ["first", "second"].
 *
 * If a occurrence is lacking a value (e.g. `--option --next-option`), it'll be
 * set to "".
 */
char** saneopt_get_all(saneopt_t* opt, const char* option);

/*
 * Get command line arguments, that is: any arguments not being an argument
 * value and all arguments after "--".
 * Return value is a NULL-terminated array.
 *
 * For example, parsing:
 *
 *   ./app --option value foo bar -- --not-option baz
 *
 * Will result in this function returning ["foo", "bar", "--not-option", "baz"].
 */
char** saneopt_arguments(saneopt_t* opt);

#ifdef __cplusplus
}
#endif

#endif
