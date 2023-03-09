#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <float.h>
#include <stddef.h>

struct alg_tuple {
  const char *a;
  const char *b;
};

struct alg_tuple_list {
  struct alg_tuple **tuples;
  size_t size;
};

struct alg_node {
  struct alg_node *next;
  struct alg_node *prev;
  float score;
  size_t size;
};

struct alg_list {
  size_t size;
  struct alg_node *first;
};

void alg_free_tuple_list(struct alg_tuple_list *list);
size_t alg_n_cr(size_t n, size_t r);
struct alg_tuple_list *alg_combine_key_pairs(char **keys, size_t key_len,
                                             size_t num_keys);
struct alg_node *alg_init_node(void);
struct alg_list *alg_init_List(void);
void alg_add_node_as_last(struct alg_node *node, struct alg_node *new);
void alg_add_node_before(struct alg_list *list, struct alg_node *node,
                         struct alg_node *new);
struct alg_list *alg_add_node(struct alg_list *list, struct alg_node *new_node);
void alg_free_list(struct alg_list *list);
unsigned int alg_hamming_dist_fixed_len(const char *hex1, const char *hex2,
                                        size_t len);
unsigned int alg_hamming_dist(const char *hex1, const char *hex2);
//void alg_rotate(unsigned char *in);
#endif /*ALGORITHMS_H*/
