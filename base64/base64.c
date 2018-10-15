#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hex.h"
#include "base64.h"


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
void b64_expand_bytes(const char* hex, char* enc, int* pos, int pad)
{
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

    enc[(*pos)++] = B64_TABLE[enc_byte_1];
    enc[(*pos)++] = B64_TABLE[enc_byte_2];
    enc[(*pos)++] = is_3rd_byte_pad_needed ? '=' : B64_TABLE[enc_byte_3];
    enc[(*pos)++] = is_4th_byte_pad_needed ? '=' : B64_TABLE[enc_byte_4];
}

/*
 * Encodes a hex string into base64. The caller of the function
 * needs to free the allocated memory for the encoded string.
 *#############################################################
 *
 * const char* hex: The pointer to the hex string to encode.
 * returns        : The pointer to the encoded string.
 */
char* b64_encode(const char* hex)
{
    // get the length
    // if its not even: return
    size_t len = strlen(hex);
    if(len % 2 != 0)
    {
        return NULL;
    }

    // allocate the needed memory
    int pad = 0;
    int pos = 0;
    // 6 chars represent 3 bytes and will be expanded to 4
    // + 1 for \0 in the end
    int bytes_to_alloc = (len / 6) * 4 + 1;
    // need to be freed by the caller
    char* enc = (char*) calloc(bytes_to_alloc, sizeof(char));

    // go through the loop decreasing the length
    // by 6. rest can be either 4 or 2
    while(len >= 6)
    {
        // do the actual encoding
        b64_expand_bytes(hex, enc, &pos, pad);
        len -=6;
        // move pointer forward
        hex +=6;
    }

    // check if we have a rest (4 or 2)
    // if we have calculate the last 4 bytes od
    // the encoded string
    pad = len % 6;
    if(pad != 0)
    {
        b64_expand_bytes(hex, enc, &pos, pad);
    }

    enc[pos++] = '\0';
    return enc;
}
