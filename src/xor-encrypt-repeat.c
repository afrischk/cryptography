#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hex.h"
#include "xor-encrypt-repeat.h"


/*
 * Encrypts the text files piped to stdin with the given key.
 * #########################################################
 *
 * const char *key: The key used for encryption. Keys that are
 *                  larger then the whole input length are trimmed.
 *
 */
void xor_encrypt_repeat(const char *key)
{
    size_t key_len = strlen(key);
    size_t buf_size = 1024;
    char in_buf[buf_size];
    // 2* because the resulting string is a hex string
    // +1 to add the null terminator
    size_t out_buf_size = 2 * buf_size + 1;
    char out_buf[out_buf_size];
    // track the global size of the input
    size_t g_pos = 0;
    char *res;
    while((res = fgets(in_buf, buf_size, stdin)) != NULL)
    {
        // out_buf position
        size_t o_pos = 0;
        // in_buf position
        size_t i_pos = 0;
        for(; i_pos < buf_size; i_pos++, g_pos++)
        {
            // if the in_buf contains a null terminator break
            if(in_buf[i_pos] == '\0')
            {
                break;
            }

            // use modulo to get the correct position in the repeating key
            char enc_byte = in_buf[i_pos] ^ key[g_pos % key_len];
            out_buf[o_pos++] = byte_to_hex(&enc_byte, true);
            out_buf[o_pos++] = byte_to_hex(&enc_byte, false);
        }

        // set the null terminator
        out_buf[o_pos++] = '\0';
        printf("%s\n", out_buf);
        // clear the buffers
        memset(out_buf, 0, out_buf_size);
        memset(in_buf, 0, buf_size);
    }
}
