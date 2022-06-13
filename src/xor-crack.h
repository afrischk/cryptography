#ifndef XOR_CRACK_H
#define XOR_CRACK_H

#include "io.h"
#include "xor-crack-types.h"

xor_crk_res_t* xor_crack_hex_str(const char *enc_hex);
xor_crk_res_t* xor_crack_bytes(data_t *data, size_t start, size_t offset);

static inline char decrypt(const char byte, const char key)
{
    return byte ^ key;
}

#endif /*XOR_CRACK_H*/
