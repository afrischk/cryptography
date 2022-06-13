#include "hamming.h"
#include "algorithms.h"
#include "io.h"
#include "score.h"
#include "xor-crack-repeat.h"
#include "xor-crack-types.h"
#include "xor-crack.h"
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

static xor_key_node_t *xor_init_key_node() {
  xor_key_node_t *node = malloc(sizeof(xor_key_node_t));
  node->key_size = 0;
  node->score = FLT_MAX;
  node->next = NULL;
  node->prev = NULL;
  return node;
}

static xor_key_list_t *init_key_List() {
  xor_key_list_t *list = malloc(sizeof(xor_key_list_t));
  list->size = 0;
  return list;
}

static void add_key_node_as_last(xor_key_node_t *node, xor_key_node_t *new) {
  node->next = new;
  new->prev = node;
}

static void add_key_node_sorted(xor_key_list_t *list, xor_key_node_t *node,
                                xor_key_node_t *new) {
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

static xor_key_list_t *add_key_node(xor_key_list_t *list,
                                    xor_key_node_t *new_node) {
  for (xor_key_node_t *current_node = list->first; current_node != NULL;
       current_node = current_node->next) {
    if (new_node->score >= current_node->score) {
      // add to end of list
      if (current_node->next == NULL) {
        add_key_node_as_last(current_node, new_node);
        break;
      }
    } else {
      add_key_node_sorted(list, current_node, new_node);
      break;
    }
  }

  list->size++;
  return list;
}

// returns a sorted list (scores) of key sizes
static xor_key_list_t *add_key_size_sorted(xor_key_list_t *list,
                                           size_t key_size, float score) {
  xor_key_node_t *new_node = xor_init_key_node();
  new_node->key_size = key_size;
  new_node->score = score;

  // empty list
  if (list->size == 0) {
    list->first = new_node;
    list->size++;
    return list;
  }

  return add_key_node(list, new_node);
}

// buggy
void free_key_list(xor_key_list_t *list) {
  xor_key_node_t *node = list->first;
  while (node->next != NULL) {
    xor_key_node_t *next = node->next;
    free(node);
    node = next;
  }
  free(node);
}

xor_key_list_t *xor_get_list_of_scored_key_sizes(const data_t *data,
                                                 size_t min_key_size,
                                                 size_t max_key_size) {
  xor_key_list_t *key_list = init_key_List();
  for (size_t key_size = min_key_size; key_size < max_key_size; key_size++) {
    char **keys = malloc(4 * sizeof(char*));
    for (size_t i = 0; i < 4; i++) {
      keys[i] = malloc(key_size * sizeof(char));
      memcpy(keys[i], data->buf + i * key_size, key_size);
    }
    tuple_t **combs = alg_combine_keys(keys, key_size, 4);
    float score = 0.0;
    for(size_t i = 0; i < alg_n_cr(4, 2); i++){
      score += (float)hamming_distance(combs[i]->a, combs[i]->b);
    }
    score /= (float)key_size;
    key_list = add_key_size_sorted(key_list, key_size, score / (float)6);
  }

  return key_list;
}

char *xor_crack_key(data_t *data, xor_key_list_t *list) {
  size_t top_n = 0;
  float key_score = 0.0;
  data_t *possible_key = malloc(sizeof(data_t));
  for (xor_key_node_t *node = list->first; node != NULL && top_n < 10;
       node = node->next) {
    size_t key_size = node->key_size;
    char *key = malloc(key_size * sizeof(char));
    for (size_t i = 0; i < key_size; i++) {
      xor_crk_res_t *res = xor_crack_bytes(data, i, key_size);
      key[i] = res->key;
      free(res);;
    }
    float tmp_score = score_text(key, key_size);
    if (tmp_score >= key_score) {
      key_score = tmp_score;
      free(possible_key->buf);
      possible_key->buf = key;
      possible_key->size = key_size;
    } else{
      free(key);
    }
    top_n++;
  }

  //printf("key len is %zu\n", possible_key->size);
  //printf("key is %s\n", possible_key->buf);
  char *dec = malloc((data->size + 1) * sizeof(char));
  memset(dec, '\0', data->size + 1);
  size_t times = data->size / possible_key->size;
  size_t rest = data->size % possible_key->size;
  for (size_t i = 0; i < times * possible_key->size; i += possible_key->size) {
    for (size_t j = 0; j < possible_key->size; j++) {
      dec[i + j] = data->buf[i + j] ^ possible_key->buf[j];
      printf("%c", dec[i + j]);
    }
  }
  for (size_t j = 0; j < rest - 1; j++) {
    dec[times * possible_key->size + j] =
        data->buf[times * possible_key->size + j] ^ possible_key->buf[j];
    printf("%c", dec[times * possible_key->size + j]);
  }
  dec[data->size + 1] = '\0';
  free(possible_key);
  return dec;
}
