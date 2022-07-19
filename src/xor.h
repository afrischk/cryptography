#ifndef XOR_CRACK_H
#define XOR_CRACK_H

#include <stddef.h>
#include "io.h"
#include "algorithms.h"

struct xor_crk_res {
  char *dec;
  float score;
  char key;
  size_t size;
};

struct xor_crk_res* xor_detect_single(const char *file_path);
struct xor_crk_res* xor_crack_hex_str(const char *enc_hex);
struct xor_crk_res* xor_crack_bytes(struct io_data *data, size_t start, size_t offset);
struct alg_list* xor_get_list_of_scored_key_sizes(const struct io_data* data, size_t min_key_size, size_t max_key_size);
void xor_encrypt_repeat(const char *key, struct io_data *data);
char* xor_crack_key(struct io_data *data, struct alg_list* list);
char* xor_encrypt(const char* hex1, const char* hex2);
void free_key_list(struct alg_list* list);

static inline char xor_decrypt(const char byte, const char key)
{
    return byte ^ key;
}

#endif /*XOR_CRACK_H*/
