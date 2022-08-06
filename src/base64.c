#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include "hex.h"
#include "base64.h"
#include "io.h"

/*
 * Transforms a 3 bytes group into a 4 bytes group by splitting
 * the 24 bits into 6 bits.
 *############################################################
 *
 * const char* hex: The pointer to the hex string to encode.
 * char* enc      : Pointer to the encoded output.
 * int* pos       : Pointer to the current position in the
 *                  encoded output.
 * int pad        : Padding that indicates a 1 or 2 byte
 *                  padding.
 */
void b64_expand_bytes(const char *hex, char *enc, int *pos, int pad) {
  // check if we need a padding of =
  // 2 means that we only have 1 byte left in the hex string
  // to encode. 4 means that we have 2 bytes left in the hex
  // string to encode.
  //
  // https://tools.ietf.org/html/rfc4648#section-4
  bool is_3rd_byte_pad_needed = pad == 2;
  bool is_4th_byte_pad_needed = is_3rd_byte_pad_needed || (pad == 4);

  // get the hex bytes
  char hex_byte_1 = hex_to_byte(hex);
  char hex_byte_2 = is_3rd_byte_pad_needed ? 0x00 : hex_to_byte(hex + 2);
  char hex_byte_3 = is_4th_byte_pad_needed ? 0x00 : hex_to_byte(hex + 4);

  // expand to 4 bytes
  char enc_byte_1 = (hex_byte_1 & 0xfc) >> 2;
  char enc_byte_2 = ((hex_byte_1 & 0x03) << 4) | ((hex_byte_2 & 0xf0) >> 4);
  char enc_byte_3 = ((hex_byte_2 & 0x0f) << 2) | ((hex_byte_3 & 0xc0) >> 6);
  char enc_byte_4 = (hex_byte_3 & 0x3f);

  enc[(*pos)++] = B64_TABLE[(unsigned char)enc_byte_1];
  enc[(*pos)++] = B64_TABLE[(unsigned char)enc_byte_2];
  enc[(*pos)++] =
      is_3rd_byte_pad_needed ? '=' : B64_TABLE[(unsigned char)enc_byte_3];
  enc[(*pos)++] =
      is_4th_byte_pad_needed ? '=' : B64_TABLE[(unsigned char)enc_byte_4];
  printf("Enc pos is %d\n", *pos);
}

/*
 * Transforms a 4 bytes group into a 3 bytes group
 *############################################################
 *
 * const char* b64: The pointer to the base64 string to decode.
 * char* dec      : Pointer to the decoded output.
 * int* pos       : Pointer to the current position in the
 * int pad        : Padding (=)
 */
void b64_collapse_bytes(const char *b64, size_t pad, bool last_4_bytes,
                        char *dec, int *pos) {
  bool has_3rd_byte_pad = pad == 2;
  bool has_4th_byte_pad = has_3rd_byte_pad || (pad == 1);

  char b64_byte_1 = b64_decode_byte(b64);
  char b64_byte_2 = b64_decode_byte(b64 + 1);
  char b64_byte_3 =
      has_3rd_byte_pad && last_4_bytes ? 0 : b64_decode_byte(b64 + 2);
  char b64_byte_4 =
      has_4th_byte_pad && last_4_bytes ? 0 : b64_decode_byte(b64 + 3);

  char dec_byte_1 = (b64_byte_1 << 2) | ((b64_byte_2 & 0x30) >> 4);
  char dec_byte_2 = ((b64_byte_2 & 0x0f) << 4) | ((b64_byte_3 & 0x3c) >> 2);
  char dec_byte_3 = ((b64_byte_3 & 0x03) << 6) | (b64_byte_4 & 0x3f);

  dec[(*pos)++] = dec_byte_1;
  dec[(*pos)++] = dec_byte_2;
  dec[(*pos)++] = dec_byte_3;
}

struct io_data *b64_decode(struct io_data *data) {
  int pos = 0;
  size_t pad = data->size % 4;
  // for every 4 bytes we only need 3
  // TODO: here is the problem
  size_t to_alloc = ((3 * (data->size - pad) / 4)) + 1;
  //size_t to_alloc = ((3 * (data->size) / 4));
  // need to be freed by the caller
  struct io_data *dec = malloc(sizeof(struct io_data));
  dec->buf = malloc(to_alloc * sizeof(char));
  // go through base64 string
  size_t bytes_to_decode = data->size - pad;
  while (bytes_to_decode >= 4) {
    bool last_4_bytes = bytes_to_decode == 4;
    b64_collapse_bytes(data->buf, pad, last_4_bytes, dec->buf, &pos);
    bytes_to_decode -= 4;
    data->buf += 4;
  }

  // go through base64 string
  dec->size = pos;
  dec->buf[pos++] = '\0';
  return dec;
}

/*
 * Encodes a hex string into base64. The caller of the function
 * needs to free the allocated memory for the encoded string.
 *#############################################################
 *
 * const char* hex: The pointer to the hex string to encode.
 * returns        : The pointer to the encoded string.
 */
char *b64_encode(const char *hex) {
  // get the length
  // if its not even: return
  size_t len = strlen(hex);
  if (len % 2 != 0) {
    return NULL;
  }

  int pad = 0;
  int pos = 0;
  int pad_alloc = 4; // TODO: define
  // allocate the needed memory
  // 6 chars represent 3 bytes and will be expanded to 4
  // + 1 for \0 in the end
  // TODO: /6??
  int bytes_to_alloc = (len/6) * 4 + pad_alloc + 1;
  // need to be freed by the caller
  char *enc = (char *)calloc(bytes_to_alloc, sizeof(char));
  // go through the loop decreasing the length
  // by 6. rest can be either 4 or 2
  while (len >= 6) {
    // do the actual encoding
    b64_expand_bytes(hex, enc, &pos, pad);
    len -= 6;
    // move pointer forward
    hex += 6;
  }
  // check if we have a rest (4 or 2)
  // if we have calculate the last 4 bytes od
  // the encoded string
  pad = len % 6;
  if (pad != 0) {
    b64_expand_bytes(hex, enc, &pos, pad);
  }

  enc[pos++] = '\0';
  return enc;
}
