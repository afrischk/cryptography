#ifndef XOR_CRACK_H
#define XOR_CRACK_H

#include "xor-crack-types.h"

xor_crk_res_t* xor_crack_hex_str(const char *enc_hex);
void xor_crack_bytes(const char *enc_bytes, size_t len, size_t start, size_t offset);

#endif /*XOR_CRACK_H*/
