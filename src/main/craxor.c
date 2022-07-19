#include "base64.h"
#include "io.h"
#include "score.h"
#include "xor.h"
#include "set-1-test.h"
#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *argp_program_version = "craxor 1.0";
static char doc[] =
    "Craxor is a tool to crack xor encryption. It takes a base64 encoded "
    "encrypted input and tries to decrpyt the cipher text.";
static char args_doc[] = "FILE";
struct arguments { char *args[1]; int run_set_tests;};
static struct argp_option options[] = {
  {"run-set-tests", 't', 0, 0, "Runs tests for set 1.", 0},
  { 0 }
};
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;
  switch (key) {
    case 't':
      arguments->run_set_tests = 1;
      break;
    case ARGP_KEY_ARG:
      if (state->arg_num >= 1 && arguments->run_set_tests != 1)
        // Too many arguments.
        argp_usage(state);
      arguments->args[state->arg_num] = arg;
      break;
    case ARGP_KEY_END:
      if (state->arg_num < 1 && arguments->run_set_tests != 1)
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
  arguments.run_set_tests = 0;

  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  if(arguments.run_set_tests == 1){
    base64_tests();
    hamming_distance_tests();
    xor_tests();
    exit(0);
  }

  struct io_data *data = read(arguments.args[0]);
  // decode
  struct io_data *dec_data = b64_decode(data);
  struct alg_list *key_list = xor_get_list_of_scored_key_sizes(dec_data, 2, 40);
  char *dec = xor_crack_key(dec_data, key_list);
  // free
  free(dec);
  free(dec_data->buf);
  free(dec_data);
  free_key_list(key_list);
  exit(0);
}
