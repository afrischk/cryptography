#include "algorithms.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void alg_free_tuple_list(struct alg_tuple_list *list) {
  for (size_t i = 0; i < list->size; i++) {
    free((void *)list->tuples[i]->a);
    list->tuples[i]->a = NULL;
    free((void *)list->tuples[i]->b);
    list->tuples[i]->b = NULL;
    free((void *)list->tuples[i]);
    list->tuples[i] = NULL;
  }
  free(list->tuples);
  list->tuples = NULL;
  free(list);
  list = NULL;
}

size_t alg_n_cr(size_t n, size_t r) {
  if (r > n) {
    return 0;
  }

  if (r == 0 || r == n) {
    return 1;
  }

  return alg_n_cr(n - 1, r - 1) + alg_n_cr(n - 1, r);
}

struct alg_tuple_list *alg_combine_key_pairs(char **keys, size_t key_len,
                                             size_t num_keys) {
  size_t num_of_key_pairs = alg_n_cr(num_keys, 2);
  struct alg_tuple_list *tuple_list = malloc(sizeof(struct alg_tuple_list));
  tuple_list->size = num_of_key_pairs;
  tuple_list->tuples = malloc(sizeof(*tuple_list->tuples) * num_of_key_pairs);
  size_t num_of_tuples_added = 0;
  while(num_of_tuples_added < num_of_key_pairs){
    for (size_t i = 0; i < num_keys; i++) {
      for (size_t j = 0; j < num_keys; j++) {
        if (i == j || i > j) {
          continue;
        }
        struct alg_tuple *t = malloc(sizeof(struct alg_tuple));
        t->a = malloc(sizeof(char) * key_len);
        t->b = malloc(sizeof(char) * key_len);
        memcpy((void *)t->a, keys[i], key_len);
        memcpy((void *)t->b, keys[j], key_len);
        tuple_list->tuples[num_of_tuples_added] = t;
        num_of_tuples_added++;
      }
    }
  }
  return tuple_list;
}
