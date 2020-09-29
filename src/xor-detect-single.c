#include <stdio.h>
#include <stdlib.h>
#include "xor-detect-single.h"

xor_crk_res_t *xor_detect_single(const char *file_path)
{
    FILE *file = fopen(file_path, "r");
    // 60 bytes needed for each string
    char hex[61];
    xor_crk_res_t *g_res = (xor_crk_res_t*) calloc(1, sizeof(xor_crk_res_t));
    g_res->score = 0.0;
    while((fscanf(file, "%60s", hex)) != EOF)
    {
        xor_crk_res_t *l_res = xor_crack_hex_str(hex);
        if(l_res->score > g_res->score)
        {
            g_res = l_res;
        }
        else
        {
            free(l_res->dec_res);
            free(l_res);
        }
    }

    fclose(file);
    return g_res;
}
