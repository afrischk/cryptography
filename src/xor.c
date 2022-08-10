#include "xor.h"
#include "algorithms.h"
#include "base64.h"
#include "hex.h"
#include "io.h"
#include "score.h"
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define NUM_ASCII_CHARS 256

// xors the hex string with a given key.
static struct io_data *xor_encrypt_hex_str(const char *enc_hex, char key,
                                           size_t len) {
  struct io_data *data = malloc(sizeof(struct io_data));
  // the decrypted messsage has len/2 because
  // we consume 2 chars of the hex string to
  // create 1 byte of the decrypted string
  size_t to_alloc = (len / 2) + 1;
  data->buf = malloc(to_alloc * sizeof(char));
  data->size = 0;
  for (size_t pos = 0; pos < len; pos += 2) {
    // xor with the key
    data->buf[data->size++] = hex_to_byte(&enc_hex[pos]) ^ key;
  }

  data->buf[data->size++] = '\0';
  return data;
}

// xors the encoded bytes
static struct xor_data *xor_encrypt_bytes_with_1_byte_key(const char *bytes,
                                                          char key, size_t len,
                                                          size_t start,
                                                          size_t block_size) {
  struct xor_data *res = malloc(sizeof(struct xor_data));
  size_t rest = ((size_t)len % block_size); // + 1;
  size_t to_alloc = ((size_t)len / block_size);
  char *dec = malloc(to_alloc * sizeof(char));
  size_t block_pos = 0;
  for (size_t pos = start; pos < (len - rest); pos += block_size) {
    // xor with the key
    dec[block_pos++] = bytes[pos] ^ key;
  }

  res->dec = dec;
  res->size = block_pos;
  return res;
}

// xor data block with given start and offset
struct xor_data *xor_get_1_byte_key_scores(struct io_data *data, size_t start,
                                           size_t block_size) {
  struct xor_data *res = malloc(sizeof(struct xor_data));
  res->score = 0.0;
  for (int key = 0; key < NUM_ASCII_CHARS; key++) {
    // decrypt the message
    struct xor_data *dec = xor_encrypt_bytes_with_1_byte_key(
        data->buf, (char)key, data->size, start, block_size);
    float score = score_text(dec->dec, dec->size);
    free(dec->dec);
    free(dec);
    if (score > res->score) {
      res->score = score;
      res->key = (char)key;
    }
  }
  return res;
}

// tries to crack the encrypted hex string by
struct xor_data *xor_crack_hex_str(const char *hex) {
  size_t len = strlen(hex);
  if (len % 2 != 0) {
    return NULL;
  }

  struct xor_data *res = malloc(sizeof(struct xor_data));
  res->dec = malloc(sizeof(char) * len / 2 + 1);
  res->score = 0.0;
  // loop through all possible values of a byte
  for (int key = 0; key < NUM_ASCII_CHARS; key++) {
    // decrypt the message
    struct io_data *dec = xor_encrypt_hex_str(hex, (char)key, len);
    // score the message
    float score = score_text(dec->buf, dec->size);
    // if we get a higher score: save the key
    if (score > res->score) {
      res->score = score;
      res->key = (char)key;
      res->size = dec->size;
      memcpy(res->dec, dec->buf, dec->size);
      free(dec->buf);
      dec->buf = NULL;
      free(dec);
      dec = NULL;
    } else {
      free(dec->buf);
      dec->buf = NULL;
      free(dec);
      dec = NULL;
    }
  }
  return res;
}

// returns a sorted list (scores) of key sizes
static struct alg_list *xor_add_scored_key_to_list(struct alg_list *list,
                                                   size_t key_size,
                                                   float score) {
  struct alg_node *new_node = alg_init_node();
  new_node->size = key_size;
  new_node->score = score;

  // empty list
  if (list->size == 0) {
    list->first = new_node;
    list->size++;
    return list;
  }

  return alg_add_node(list, new_node);
}

static void xor_free_keys(char **keys) {
  for (size_t i = 0; i < 4; i++) {
    free(keys[i]);
    keys[i] = NULL;
  }
  free(keys);
  keys = NULL;
}

struct alg_list *xor_get_list_of_scored_key_sizes(const struct io_data *data,
                                                  size_t min_key_size,
                                                  size_t max_key_size) {
  struct alg_list *key_list = alg_init_List();
  for (size_t key_size = min_key_size; key_size < max_key_size; key_size++) {
    char **keys = malloc(4 * sizeof(char *));
    for (size_t i = 0; i < 4; i++) {
      keys[i] = malloc(key_size * sizeof(char));
      memcpy(keys[i], data->buf + i * key_size, key_size);
    }
    struct alg_tuple_list *combs = alg_combine_key_pairs(keys, key_size, 4);
    float score = 0.0;
    for (size_t i = 0; i < combs->size; i++) {
      score += (float)alg_hamming_dist_fixed_len(combs->tuples[i]->a,
                                                 combs->tuples[i]->b, key_size);
    }
    score /= (float)key_size;
    key_list = xor_add_scored_key_to_list(key_list, key_size, score / (float)6);
    alg_free_tuple_list(combs);
    xor_free_keys(keys);
  }

  return key_list;
}

char *xor_encrypt_bytes_with_n_bytes_key(struct io_data *data,
                                         struct io_data *key) {
  char *dec = malloc((data->size + 1) * sizeof(char));
  memset(dec, '\0', data->size + 1);
  size_t times = data->size / key->size;
  size_t rest = data->size % key->size;
  for (size_t i = 0; i < times * key->size; i += key->size) {
    for (size_t j = 0; j < key->size; j++) {
      dec[i + j] = data->buf[i + j] ^ key->buf[j];
    }
  }
  for (size_t j = 0; j < rest - 1; j++) {
    dec[times * key->size + j] = data->buf[times * key->size + j] ^ key->buf[j];
  }

  // free
  free(key->buf);
  key->buf = NULL;
  free(key);
  key = NULL;
  free(data->buf);
  data->buf = NULL;
  free(data);
  data = NULL;
  return dec;
}

char *xor_encrypt_data_with_most_likely_key(struct io_data *data,
                                            struct alg_list *list) {
  size_t top_n = 0;
  float key_score = 0.0;
  struct io_data *possible_key = malloc(sizeof(struct io_data));
  possible_key->buf = malloc(sizeof(char));
  for (struct alg_node *node = list->first; node != NULL && top_n < 10;
       node = node->next) {
    size_t key_size = node->size;
    char *key = malloc(key_size * sizeof(char));
    for (size_t i = 0; i < key_size; i++) {
      struct xor_data *res = xor_get_1_byte_key_scores(data, i, key_size);
      key[i] = res->key;
      free(res);
    }
    float tmp_score = score_text(key, key_size);
    if (tmp_score >= key_score) {
      key_score = tmp_score;
      possible_key->buf = realloc(possible_key->buf, key_size);
      memcpy(possible_key->buf, key, key_size);
      possible_key->size = key_size;
    }
    free(key);
    top_n++;
  }

  alg_free_list(list);

  return xor_encrypt_bytes_with_n_bytes_key(data, possible_key);
}

struct xor_data *xor_crack_with_most_likely_1_byte_key(const char *file_path) {
  FILE *file = fopen(file_path, "r");
  // 60 bytes needed for each string
  char hex[61];
  struct xor_data *g_res = malloc(sizeof(struct xor_data));
  g_res->score = 0.0;
  g_res->dec = NULL;
  while ((fscanf(file, "%60s", hex)) != EOF) {
    struct xor_data *l_res = xor_crack_hex_str(hex);
    if (l_res->score > g_res->score) {
      if (g_res->dec != NULL) {
        free(g_res->dec);
        g_res->dec = NULL;
      }
      g_res->dec = malloc(l_res->size * sizeof(char));
      memcpy(g_res->dec, l_res->dec, l_res->size);
      g_res->size = l_res->size;
      g_res->score = l_res->score;
      g_res->key = l_res->key;
    }
    free(l_res->dec);
    free(l_res);
  }

  fclose(file);
  return g_res;
}

char *xor_encrypt_with_n_bytes_key_to_hex_str(struct io_data *data,
                                              const char *key) {
  size_t key_len = strlen(key);
  //  2* because the resulting string is a hex string
  //  +1 to add the null terminator
  size_t out_buf_size = 2 * data->size + 1;
  char *out_buf = malloc(out_buf_size * sizeof(char));
  // track the global size of the input
  size_t g_pos = 0;
  //  out_buf position
  size_t o_pos = 0;
  // in_buf position
  size_t i_pos = 0;
  for (; i_pos < data->size; i_pos++, g_pos++) {
    // use modulo to get the correct position in the repeating key
    char enc_byte = data->buf[i_pos] ^ key[g_pos % key_len];
    out_buf[o_pos++] = byte_to_hex(&enc_byte, true);
    out_buf[o_pos++] = byte_to_hex(&enc_byte, false);
  }
  // set the null terminator
  out_buf[o_pos++] = '\0';
  return out_buf;
}

// xor encodes two strings of equal size.
char *xor_encrypt_2_hex_str(const char *hex1, const char *hex2) {
  size_t hex1_len = strlen(hex1);
  size_t hex2_len = strlen(hex2);

  if (hex1_len != hex2_len)
    return NULL;
  if (hex1_len % 2 != 0)
    return NULL;

  unsigned int pos = 0;
  char *enc = (char *)calloc(hex1_len + 1, sizeof(char));
  while (hex1_len >= 2) {
    // example:
    // 1c ^ 68 = 74
    // 0001 1100
    // 0110 1000
    // ---------
    // 0111 0100
    char xor_enc_byte = hex_to_byte(hex1) ^ hex_to_byte(hex2);
    // true means to take the 4 high bits
    enc[pos++] = byte_to_hex(&xor_enc_byte, true);
    // false means to take the low 4 bits
    enc[pos++] = byte_to_hex(&xor_enc_byte, false);
    hex1_len -= 2;
    hex1 += 2;
    hex2 += 2;
  }

  // finish
  enc[pos++] = '\0';
  return enc;
}

char *xor_crack(struct io_data *data, unsigned int min_key_len,
                unsigned int max_key_len) {
  struct io_data *dec_data = b64_decode(data);
  struct alg_list *key_list =
      xor_get_list_of_scored_key_sizes(dec_data, min_key_len, max_key_len);
  return xor_encrypt_data_with_most_likely_key(dec_data, key_list);
}
