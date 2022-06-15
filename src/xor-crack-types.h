#ifndef XOR_CRACK_TYPES_H
#define XOR_CRACK_TYPES_H

#include <stddef.h>

struct xor_crk_res {
  char *dec;
  float score;
  char key;
  size_t size;
};

#endif /*XOR_CRACK_TYPES_H*/
