#include <stdio.h>
#include <stdlib.h>
#include "xor-detect.h"
#include "xor-crack.h"

int main()
{
    xor_crk_res_t *res = xor_detect("xor-encrypted-hex.txt");

    printf("Plaintext: %s", res->dec_res);
    printf("Key: %c\n", res->key);
    printf("Score: %f\n", res->score);

    free(res->dec_res);
    free(res);

    return 0;
}
