#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <stddef.h>

typedef struct tuple {
  const char *a;
  const char *b;
  size_t size; // TODO currently not used
} tuple_t;

size_t alg_n_cr(size_t n, size_t r);
tuple_t **alg_combine_keys(char **keys, size_t key_len, size_t num_keys);

#endif /*ALGORITHMS_H*/
