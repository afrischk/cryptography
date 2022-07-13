#ifndef BASE64_H
#define BASE64_H

#include <stdbool.h>
#include <stddef.h>
#include "io.h"

char *b64_encode(const char *hex);
struct io_data *b64_decode(struct io_data *data);
// void b64_decode_file(const char* file_name_in, const char* file_name_out);
void b64_expand_bytes(const char *hex, char *enc, int *pos, int pad);
void b64_collapse_bytes(const char *b64, size_t pad, bool last_4_bytes, char *dec,
                        int *pos);

static const unsigned char B64_TABLE[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

static const unsigned char B64_DECODE_TABLE[] = {
    62, 0,  0,  0,  63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0,
    0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
    0,  0,  0,  0,  0,  0,  26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
    36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51};

/*
 * Converts the b64 char into the encoded byte.
 * ##################################
 *
 * char* b64: b64 char
 * returns: resulting byte value
 */
static inline unsigned char b64_decode_byte(const char *b64) {
  return B64_DECODE_TABLE[(*b64) - '+'];
}

#endif /*BASE64_H*/
