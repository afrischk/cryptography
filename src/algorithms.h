#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <stddef.h>

struct alg_tuple {
  const char *a;
  const char *b;
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

size_t alg_n_cr(size_t n, size_t r);
struct alg_tuple **alg_combine_keys(char **keys, size_t key_len, size_t num_keys);


/*
 * Calculates hamming distance between 2 (hex)
 * strings.
 * ###########################################
 *
 *  const char* hex1: pointer to the hex string.
 *  const char* hex2: pointer to the hex string.
 *  returns: hamming distance
 */
static inline unsigned int alg_hamming_distance(const char* hex1, const char* hex2)
{
    unsigned int distance = 0;
    // works for strings of equal length
    while(*hex1)
    {
        unsigned int xor_byte = *hex1++ ^ *hex2++;
        while (xor_byte > 0)
        {
            distance += xor_byte & 1;
            xor_byte >>= 1;
        }
    }

    return distance;
}
#endif /*ALGORITHMS_H*/
