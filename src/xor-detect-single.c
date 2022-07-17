#include "xor-detect-single.h"
#include "xor-crack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct xor_crk_res *xor_detect_single(const char *file_path) {
  FILE *file = fopen(file_path, "r");
  // 60 bytes needed for each string
  char hex[61];
  struct xor_crk_res *g_res = malloc(sizeof(struct xor_crk_res));
  g_res->score = 0.0;
  g_res->dec =NULL;
  while ((fscanf(file, "%60s", hex)) != EOF) {
    struct xor_crk_res *l_res = xor_crack_hex_str(hex);
    if (l_res->score > g_res->score) {
      if(g_res->dec != NULL){
        free(g_res->dec);
        g_res->dec = NULL;
      }
      g_res->dec = malloc(l_res->size * sizeof(char));
      memcpy(g_res->dec, l_res->dec, l_res->size);
      g_res->size = l_res->size;
      g_res->score = l_res->score;
      g_res->key = l_res->key;
    }
    free(l_res->dec);
    free(l_res);
  }

  fclose(file);
  return g_res;
}
