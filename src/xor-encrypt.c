#include <string.h>
#include <stdlib.h>
#include "hex.h"
#include "xor-encrypt.h"
/*
 * XOR encodes two strings of equal size.
 * #####################################
 *
 * const char* hex1: 1st hex encoded string.
 * const char* hex2: 2nd hex encoded string.
 * returns: XOR encoded string.
 */
char* xor_encrypt(const char* hex1, const char* hex2)
{
    size_t hex1_len = strlen(hex1);
    size_t hex2_len = strlen(hex2);

    if(hex1_len != hex2_len) return NULL;
    if(hex1_len % 2 != 0) return NULL;

    unsigned int pos = 0;
    char* enc = (char*) calloc(hex1_len + 1, sizeof(char));
    while(hex1_len >= 2)
    {
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
