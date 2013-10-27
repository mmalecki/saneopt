#ifndef __SANEOPT_H
#define __SANEOPT_H

typedef struct saneopt_option_s {
  char* name;
  int required;
  int available;
  int requires_value;
  char* desc;
  char* help;
  char* value;

  char** aliases;
  int aliases_length;
} saneopt_option_t;

typedef struct saneopt_s {
  int argc;
  char** argv;

  char* header;

  saneopt_option_t** options;
  int options_length;

  char** arguments;
  int arguments_length;
} saneopt_t;

/*
 *  Create an short option alias for a long option
 */
saneopt_option_t* saneopt_alias(saneopt_option_t* option, char* alias);

/*
 *  Search for the short or long option in the current list of options
 */
saneopt_option_t* saneopt_get(saneopt_t* saneopt, char* input, int _long);

/*
 *  Output the help for option
 *  If option is NULL, it will print the descriptions for all options
 */
void saneopt_help(saneopt_t* saneopt, saneopt_option_t* option);

/*
 *  Initialize and return saneopt for assigning options, aliases, etc.
 */
saneopt_t* saneopt_init();

/*
 *  Free any malloc'd objects
 */
void saneopt_free(saneopt_t* saneopt);

/*
 *  Add option. If option already exists, the option will be returned.
 */
saneopt_option_t* saneopt_option(saneopt_t* saneopt, char* option);

/*
 *  Parse the inputs from main()
 *  If saneopt is NULL, a new saneopt will be created with saneopt_init()
 */
saneopt_t* saneopt_parse(saneopt_t* saneopt, int argc, char** argv);

/*
 *  Allows an argument to be manually pushed into the arguments list
 */
saneopt_t* saneopt_push_argument(saneopt_t* saneopt, char* argument);

#endif //__SANEOPT_H
