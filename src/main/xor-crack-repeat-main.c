#include "base64.h"
#include "io.h"
#include "score.h"
#include "xor-crack-repeat.h"
#include "xor-crack-types.h"
#include <argp.h>
#include <stdio.h>
#include <stdlib.h>

const char *argp_program_version = "craxor 1.0";
static char doc[] =
    "Craxor is a tool to crack xor encryption. It takes a base64 encoded "
    "encrypted input and tries to decrpyt the cipher text.";
static char args_doc[] = "FILE";
struct arguments { char *args[1]; };

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;

  switch (key) {
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

static struct argp argp = {0, parse_opt, args_doc, doc, 0, 0, 0};

int main(int argc, char **argv) {
  struct arguments arguments;
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  // hamming distance test
  const char *s1 = "this is a test";
  const char *s2 = "wokka wokka!!!";
  unsigned int distance = alg_hamming_distance(s1, s2);
  printf("Hamming distance is: %d\n", distance);
  const char *str = "wokka wokka";
  float score = score_text(str, 11);
  printf("Score is %f\n", score);
  printf("Start cracking...\n");
  // malloc read data
  struct io_data *data = malloc(sizeof(struct io_data));
  // read from stdin
  data = read(arguments.args[0]); // TODO warning about prototyping...
  // decode
  struct io_data *dec_data = b64_decode(data);
  struct alg_list *key_list = xor_get_list_of_scored_key_sizes(dec_data, 2, 41);
  xor_crack_key(dec_data, key_list);
  // free
  free(data);
  free(key_list);
  free(dec_data);
  exit(0);
}
