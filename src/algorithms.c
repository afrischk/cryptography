#include "algorithms.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// calculates hamming distance between 2 (hex)
unsigned int alg_hamming_dist_fixed_len(const char *hex1, const char *hex2,
                                        size_t len) {
  unsigned int distance = 0;
  // works for strings of equal length
  for (size_t i = 0; i < len; i++) {
    unsigned int xor_byte = hex1[i] ^ hex2[i];
    while (xor_byte > 0) {
      distance += xor_byte & 1;
      xor_byte >>= 1;
    }
  }
  return distance;
}

unsigned int alg_hamming_dist(const char *hex1, const char *hex2) {
  unsigned int distance = 0;
  // works for strings of equal length
  while (*hex1) {
    unsigned int xor_byte = *hex1++ ^ *hex2++;
    while (xor_byte > 0) {
      distance += xor_byte & 1;
      xor_byte >>= 1;
    }
  }

  return distance;
}

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
  while (num_of_tuples_added < num_of_key_pairs) {
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

struct alg_node *alg_init_node(void) {
  struct alg_node *node = malloc(sizeof(struct alg_node));
  node->size = 0;
  node->score = FLT_MAX;
  node->next = NULL;
  node->prev = NULL;
  return node;
}

struct alg_list *alg_init_List(void) {
  struct alg_list *list = malloc(sizeof(struct alg_list));
  list->size = 0;
  return list;
}

void alg_add_node_as_last(struct alg_node *node, struct alg_node *new) {
  node->next = new;
  new->prev = node;
}

void alg_add_node_before(struct alg_list *list, struct alg_node *node,
                         struct alg_node *new) {
  new->next = node;
  new->prev = node->prev;
  node->prev = new;
  if (new->prev == NULL) {
    // if no prev available its the first node
    list->first = new;
  } else {
    new->prev->next = new;
  }
}

struct alg_list *alg_add_node(struct alg_list *list,
                              struct alg_node *new_node) {
  for (struct alg_node *current_node = list->first; current_node != NULL;
       current_node = current_node->next) {
    if (new_node->score >= current_node->score) {
      // add to end of list
      if (current_node->next == NULL) {
        alg_add_node_as_last(current_node, new_node);
        break;
      }
    } else {
      alg_add_node_before(list, current_node, new_node);
      break;
    }
  }

  list->size++;
  return list;
}

void alg_free_list(struct alg_list *list) {
  struct alg_node *node = list->first;
  while (node->next != NULL) {
    struct alg_node *next = node->next;
    free(node);
    node = next;
  }
  free(node);
  free(list);
}

/*void alg_rotate(unsigned char *in, size_t len){
    unsigned char a;
    size_t i = 0;
    for (i < len; i++){
        in[i] = in[i + 1]
    }
    in[len - 1] = a;
}*/
