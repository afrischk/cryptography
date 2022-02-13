#include "hamming.h"
#include "xor-crack-types.h"
#include "xor-crack.h"
#include <errno.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

extern int errno;

typedef struct tuple {
  const char *a;
  const char *b;
  size_t size;
} tuple_t;

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

xor_key_list_t *xor_get_list_of_scored_key_sizes(const char *byte_file,
                                                 size_t min_key_size,
                                                 size_t max_key_size) {
  FILE *in = fopen(byte_file, "rb");
  if (in == NULL) {
    fprintf(stderr, "Error occured: %d Could not open file: %s\n", errno,
            strerror(errno));
  }

  int fd = fileno(in);
  struct stat buf;
  fstat(fd, &buf);
  off_t file_size = buf.st_size;

  printf("File size is %li \n", file_size);

  char *buffer = malloc(file_size * sizeof(char) + 1);
  // read max_key_size worth of bytes
  // its all we need to calculate the key
  size_t bytes_read = fread(buffer, 1, file_size, in);
  if (bytes_read < file_size) {
    fprintf(stderr, "Not enough bytes in file. Took %zu bytes.\n", bytes_read);
  }

  buffer += '\0';

  size_t pos = 0;
  size_t elements = 0;
  float key_size_score = 0.0;
  xor_key_list_t *key_list = init_key_List();
  for (size_t key_size = min_key_size; key_size < max_key_size; key_size++) {
    printf("Start with key size %zu \n", key_size);
    while (pos + 2 * key_size < bytes_read) {

      char key1[key_size + 1];
      char key2[key_size + 1];

      memset(key1, '\0', sizeof(key1));
      memset(key2, '\0', sizeof(key2));
      //char *key1 = malloc(key_size * sizeof(char) + 1);
      //char *key2 = malloc(key_size * sizeof(char) + 1);

      printf("Current pos is %zu \n", pos);

      strncpy(key1, buffer + pos, key_size);
      strncpy(key2, buffer + pos + key_size, key_size);
      //key1 += '\0';
      //key2 += '\0';

      unsigned int distance = hamming_distance(key1, key2);

      printf("Distance is %i \n", distance);

      float score = (float)distance / (float)key_size;

      printf("Normalized score is %f \n", score);

      key_size_score += score;

      printf("Overall score is %f \n", key_size_score);

      pos += 2 * key_size;

      printf("Next pos is %zu \n", pos);

      elements++;

      //free(key1);
      //free(key2);
    }

    key_list =
        add_key_size_sorted(key_list, key_size, key_size_score / elements);
    pos = 0;
    key_size_score = 0;
    elements = 0;
  }

  free(buffer);
  fclose(in);
  return key_list;
}

char *xor_crack_key(const char *byte_file, xor_key_list_t *list) {
  FILE *in = fopen(byte_file, "rb");
  if (in == NULL) {
    fprintf(stderr, "Error occured: %d Could not open file: %s\n", errno,
            strerror(errno));
  }

  fseek(in, 0, SEEK_END);
  size_t fsize = ftell(in);
  fseek(in, 0, SEEK_SET);

  char *bytes = malloc(fsize * sizeof(char));
  fread(bytes, 1, fsize, in);

  for (xor_key_node_t *node = list->first; node != NULL; node = node->next) {
    size_t key_size = node->key_size;
    printf("Start cracking with key size %zu\n", key_size);
    size_t blocks = (size_t)fsize/key_size;
    char transposed_block[blocks];
    for (size_t i = 0; i < key_size; i++){
      for(size_t j = 0; j < blocks; j++){
        transposed_block[j] = bytes[(key_size * j) + i];
      }
    }

    // ignore
    char *key = malloc((key_size + 1) * sizeof(char));
    for (size_t i = 0; i < key_size; i++) {
      xor_crk_res_t *res = xor_crack_bytes(bytes, fsize, i, key_size);
      printf("Score: %f with key: %c\n", res->score, res->key);
      key[i] = res->key;
      free(res);
    }
    key[key_size + 1] = '\0';
    printf("key is %s\n", key);

    char *dec = malloc((fsize + 1) * sizeof(char *));
    for (size_t i = 0; i < fsize; i += key_size) {
      for (size_t j = 0; j < key_size; j++) {
        dec[i + j] = key[j] ^ bytes[i + j];
      }
    }
    dec[fsize + 1] = '\0';
    printf("dec is %s\n", dec);
    free(key);
    free(dec);
  }
  // free(block);
  free(bytes);
  fclose(in);
  return "";
}
