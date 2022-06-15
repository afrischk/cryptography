#include "base64.h"
#include "io.h"
#include "score.h"
#include "xor-crack-repeat.h"
#include "xor-crack-types.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
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
  data = read(); // TODO warning about prototyping...
  // decode
  struct io_data *dec_data = b64_decode(data);
  struct alg_list *key_list = xor_get_list_of_scored_key_sizes(dec_data, 2, 41);
  xor_crack_key(dec_data, key_list);
  // free
  free(data);
  free(key_list);
  free(dec_data);
  return 0;
}
