#ifndef XOR_CRACK_H
#define XOR_CRACK_H

#include "io.h"
#include "xor-crack-types.h"

struct xor_crk_res* xor_crack_hex_str(const char *enc_hex);
struct xor_crk_res* xor_crack_bytes(struct io_data *data, size_t start, size_t offset);

static inline char xor_decrypt(const char byte, const char key)
{
    return byte ^ key;
}

#endif /*XOR_CRACK_H*/
