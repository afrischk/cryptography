#include "base64.h"
#include "io.h"
#include "score.h"
#include "algorithms.h"
#include "xor.h"
#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *argp_program_version = "craxor 1.0";
static char doc[] =
    "Craxor is a tool to crack xor encrypted data. It takes a base64 encoded "
    "input and tries to decrpyt the cipher text.";
static char args_doc[] = "FILE";
struct arguments {
  char *args[1];
  unsigned int min_key_len;
  unsigned int max_key_len;
};
static struct argp_option options[] = {
    {"min_key_len", 'a', "MINKEYLEN", OPTION_ARG_OPTIONAL, "Minimum assumend key length (default: 2).", 0},
    {"max_key_len", 'b', "MAXKEYLEN", OPTION_ARG_OPTIONAL, "Maximum assumend key length (default: 40).", 0},
    {0}};
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;
  switch (key) {
  case 'a':
    arguments->min_key_len = atoi((const char *)arg);
    break;
  case 'b':
    arguments->max_key_len = atoi((const char *)arg);
    break;
  case ARGP_KEY_ARG:
    if (state->arg_num >= 1)
      // Too many arguments.
      argp_usage(state);
    arguments->args[state->arg_num] = arg;
    break;
  case ARGP_KEY_END:
    if (state->arg_num < 1)
      // Not enough arguments.
      argp_usage(state);
    break;
  default:
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int main(int argc, char **argv) {
  struct arguments arguments;
  arguments.min_key_len = 2;
  arguments.max_key_len = 40;

  if(arguments.min_key_len == arguments.max_key_len){
    printf("Min and max assumed key lengths cannot be equal.");
    exit(0);
  }

  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  struct io_data *data = read(arguments.args[0]);
  char *dec = xor_crack(data, arguments.min_key_len, arguments.max_key_len);
  printf("%s", dec);
  // free
  free(dec);
  exit(0);
}
