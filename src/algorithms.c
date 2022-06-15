#include <stdlib.h>
#include "algorithms.h"

size_t alg_n_cr(size_t n, size_t r) {
  if (r > n) {
    return 0;
  }

  if (r == 0 || r == n) {
    return 1;
  }

  return alg_n_cr(n - 1, r - 1) + alg_n_cr(n - 1, r);
}

struct alg_tuple **alg_combine_keys(char **keys, size_t key_len, size_t num_keys) {
  struct alg_tuple **combinations = malloc(alg_n_cr(num_keys, 2) * sizeof(*combinations));
  size_t x = 0;
  for (size_t i = 0; i < num_keys; i++) {
    for (size_t j = 0; j < num_keys; j++) {
      if(i == j || i > j){
        continue;
      }
      struct alg_tuple *t = malloc(sizeof(struct alg_tuple));
      t->a = malloc(sizeof(char) * key_len);
      t->b = malloc(sizeof(char) * key_len);
      memcpy(t->a, keys[i], key_len);
      memcpy(t->b, keys[j], key_len);
      combinations[x++] = t;
      // TODO why does pointer arithemtic not work?
      //*combinations = t;
    }
  }
  return combinations;
}
