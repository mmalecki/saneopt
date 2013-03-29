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
saneopt_t* saneopt_new(int argc, char** argv);

/*
 * Set an alias from `old` to `new`.
 */
void saneopt_alias(saneopt_t* opt, char* old, char* new_);

/*
 * Get option called `option`.
 */
char* saneopt_get(saneopt_t* opt, char* name);

#endif
