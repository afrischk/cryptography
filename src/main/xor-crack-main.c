#include <stdio.h>
#include <stdlib.h>
#include "xor-crack.h"

static void free_xor_crk_res_t(xor_crk_res_t *res);

int main()
{
    const char* enc = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    xor_crk_res_t *res = xor_crack(enc);
    printf("The decrypted message is: %s\n", res->dec_res);
    printf("With score: %f\n", res->score);
    printf("And key: %c\n", res->key);
    free_xor_crk_res_t(res);
    return 0;
}

static void free_xor_crk_res_t(xor_crk_res_t *res)
{
    free(res->dec_res);
    free(res);
}
