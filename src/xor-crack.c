#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hex.h"
#include "io.h"
#include "score.h"
#include "xor-crack.h"

/*
 * Decrypts the hex string with a given key.
 * ########################################
 *
 * const char* enc_hex: The hex string to decrypt
 * char key: The key used for decryption
 * size_t len: The length of the hex string
 * returns: The decrypted text.
 *
 */
static char *decrypt_hex_str(const char *enc_hex, char key, size_t len)
{
    // the decrypted messsage has len/2 because
    // we consume 2 chars of the hex string to
    // create 1 byte of the decrypted string
    size_t to_alloc = (len / 2) + 1;
    char *dec = (char *)calloc(to_alloc, sizeof(char));
    for (size_t pos = 0, dec_pos = 0; pos < len; pos += 2)
    {
        // xor with the key
        dec[dec_pos] = decrypt(hex_to_byte(&enc_hex[pos]), key);
        dec_pos++;
    }
    dec[to_alloc] = '\0';
    return dec;
}

static xor_crk_res_t *decrypt_bytes(const char *enc_bytes, char key, size_t len, size_t start, size_t offset)
{
    xor_crk_res_t *res = malloc(sizeof(xor_crk_res_t));
    size_t to_alloc = ((size_t)len/offset);// + 1;
    char *dec = malloc(to_alloc * sizeof(char));
    size_t block_pos = 0;
    for (size_t pos = start; pos < len; pos += offset)
    {
        // xor with the key
        dec[block_pos++] = decrypt(enc_bytes[pos], key);
    }

    res->dec_res = dec;
    res->len = block_pos;
    //dec[to_alloc] = '\0';
    return res;
}

xor_crk_res_t *xor_crack_bytes(data_t *data, size_t start, size_t offset)
{
    xor_crk_res_t *res = malloc(sizeof(xor_crk_res_t));
    res->score = 0.0;
    for (int key = 0; key <= 255; key++)
    {
        // decrypt the message
        xor_crk_res_t *dec = decrypt_bytes(data->buf, (char)key, data->size, start, offset);
        float score = score_text(dec->dec_res, dec->len);
        free(dec->dec_res);
        free(dec);
        if (score > res->score){
            res->score = score;
            res->key = (char)key;
        }
    }
    return res;
}

/*
 * Tries to crack the encrypted hex string by
 * XORing one byte keys ranging from 0-255.
 * #########################################
 *
 * const char* enc_hex: The encrypted hex string
 * returns: The decrypted message.
 *
 */
xor_crk_res_t *xor_crack_hex_str(const char *enc_hex)
{
    size_t len = strlen(enc_hex);
    if (len % 2 != 0)
    {
        return NULL;
    }

    xor_crk_res_t *res = (xor_crk_res_t *)calloc(1, sizeof(xor_crk_res_t));
    res->score = 0.0;
    // loop through all possible values of a byte
    for (int key = 0; key <= 255; key++)
    {
        // decrypt the message
        char *dec = decrypt_hex_str(enc_hex, (char)key, len);
        // score the message
        float score = score_text(dec, len / 2);
        // if we get a higher score: save the key
        if (score > res->score)
        {
            res->score = score;
            res->key = (char)key;
            res->dec_res = dec;
        }
        else
        {
            free(dec);
        }
    }

    return res;
}
