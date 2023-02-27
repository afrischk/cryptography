#ifndef XOR_H
#define XOR_H

#include <stddef.h>
#include "io.h"
#include "algorithms.h"

struct xor_data {
  char *dec;
  float score;
  char key;
  size_t size;
};

struct xor_data* xor_crack_with_most_likely_1_byte_key(const char *file_path);
struct xor_data* xor_crack_hex_str(const char *enc_hex);
struct xor_data* xor_get_1_byte_key_scores(struct io_data *data, size_t start, size_t block_size);
struct alg_list* xor_get_list_of_scored_key_sizes(const struct io_data* data, size_t min_key_size, size_t max_key_size);
char* xor_encrypt_data_with_most_likely_key(struct io_data *data, struct alg_list *list);
char* xor_encrypt_2_hex_str(const char* hex1, const char* hex2);
char* xor_encrypt_bytes_with_n_bytes_key(struct io_data *data, struct io_data *key);
char* xor_encrypt_2_byte_arrays(char *bytes1, char *bytes2, size_t len);
char* xor_encrypt_with_n_bytes_key_to_hex_str(struct io_data *data, const char *key);
char* xor_crack(struct io_data *data, unsigned int min_key_len, unsigned int max_key_len);

#endif /*XOR_H*/
