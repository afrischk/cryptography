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

static struct alg_node* xor_init_key_node() {
  struct alg_node *node = malloc(sizeof(struct alg_node));
  node->size = 0;
  node->score = FLT_MAX;
  node->next = NULL;
  node->prev = NULL;
  return node;
}

static struct alg_list* init_key_List() {
  struct alg_list*list = malloc(sizeof(struct alg_list));
  list->size = 0;
  return list;
}

static void add_key_node_as_last(struct alg_node *node, struct alg_node *new) {
  node->next = new;
  new->prev = node;
}

static void add_key_node_sorted(struct alg_list *list, struct alg_node *node,
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

static struct alg_list *add_key_node(struct alg_list *list,
                                    struct alg_node *new_node) {
  for (struct alg_node *current_node = list->first; current_node != NULL;
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
static struct alg_list *add_key_size_sorted(struct alg_list *list,
                                           size_t key_size, float score) {
  struct alg_node *new_node = xor_init_key_node();
  new_node->size = key_size;
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
void free_key_list(struct alg_list *list) {
  struct alg_node *node = list->first;
  while (node->next != NULL) {
    struct alg_node *next = node->next;
    free(node);
    node = next;
  }
  free(node);
  free(list);
}

static void free_keys(char **keys){
  for(size_t i = 0; i < 4; i++){
    free(keys[i]);
    keys[i] = NULL;
  }
  free(keys);
  keys = NULL;
}

struct alg_list *xor_get_list_of_scored_key_sizes(const struct io_data *data,
                                                 size_t min_key_size,
                                                 size_t max_key_size) {
  struct alg_list *key_list = init_key_List();
  for (size_t key_size = min_key_size; key_size < max_key_size; key_size++) {
    char **keys = malloc(4 * sizeof(char*));
    for (size_t i = 0; i < 4; i++) {
      keys[i] = malloc(key_size * sizeof(char));
      memcpy(keys[i], data->buf + i * key_size, key_size);
    }
    struct alg_tuple_list *combs = alg_combine_key_pairs(keys, key_size, 4);
    float score = 0.0;
    for(size_t i = 0; i < combs->size; i++){
      score += (float)alg_hamming_dist_fixed_len(combs->tuples[i]->a, combs->tuples[i]->b, key_size);
    }
    score /= (float)key_size;
    key_list = add_key_size_sorted(key_list, key_size, score / (float)6);
    alg_free_tuple_list(combs);
    free_keys(keys);
  }

  return key_list;
}

char* xor_crack_key(struct io_data *data, struct alg_list *list) {
  size_t top_n = 0;
  float key_score = 0.0;
  struct io_data *possible_key = malloc(sizeof(struct io_data));
  possible_key->buf = malloc(sizeof(char));
  for (struct alg_node *node = list->first; node != NULL && top_n < 10;
       node = node->next) {
    size_t key_size = node->size;
    char *key = malloc(key_size * sizeof(char));
    for (size_t i = 0; i < key_size; i++) {
      struct xor_crk_res *res = xor_crack_bytes(data, i, key_size);
      key[i] = res->key;
      free(res);
    }
    float tmp_score = score_text(key, key_size);
    if (tmp_score >= key_score) {
      key_score = tmp_score;
      // TODO why not free?
      //free(possible_key->buf);
      possible_key->buf = realloc(possible_key->buf, key_size);
      memcpy(possible_key->buf, key, key_size);
      possible_key->size = key_size;
    }
    free(key);
    top_n++;
  }

  // TODO write crack function
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
  //dec[data->size + 1] = '\0';
  free(possible_key->buf);
  free(possible_key);
  //free(data->buf);
  //free(data);
  return dec;
}
