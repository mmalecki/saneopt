#include <stdio.h>
#include <saneopt.h>

int main(int argc, char** argv) {
  saneopt_t* saneopt = saneopt_init();

  saneopt->header = "saneopt binary";

  saneopt_option_t* host = saneopt_option(saneopt, "host");
  saneopt_alias(host, "h");
  host->desc = "Define host";

  saneopt_option_t* version = saneopt_option(saneopt, "version");
  saneopt_alias(version, "v");
  version->desc = "Return version and exit";
  version->requires_value = 0;

  saneopt_parse(saneopt, argc, argv);

  if (version->available) {
    printf("%s\n", SANEOPT_VERSION);
  }
  else if (host->value != NULL) {
    printf("Host: %s\n", host->value);
  }

  saneopt_free(saneopt);

  return 0;
}
