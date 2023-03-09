#include "aes.h"
#include "xor.h"
#include <stddef.h>
#include <string.h>

/*void xor_2_words(word w1, word w2, word res, size_t len){
  for(size_t i = 0; i < len; i++){
    res[i] = w1[i] ^ w2[2];
  }
}*/

// word is a 4 byte array of size nb * (nr + 1)
// nk is the key length with size 4 * nk
// nb is the block size of 4 (columns)
void aes_expand_key(char const *key, char **words, size_t nk, size_t nr) {

  char temp[AES_WORD_SIZE];
  size_t i = 0;

  while (i < nk) {
    for (size_t j = 0; j < 4; j++) {
      words[i][j] = key[4 * i + j];
    }
    i++;
  }

  i = nk;

  while (i < AES_BLOCK_SIZE * (nr + 1)) {
    // temp = &w[i - 1];
    memcpy(temp, words[i - 1], AES_WORD_SIZE);

    if (i % nk == 0) {
      // temp = aes_sub_word(aes_rotate_word(temp)) ^
      // aes_round_constant(unsigned char i);

    } else if (nk > 6 && i % nk == 4) {
      // temp = aes_sub_word(temp);
    }
    // char *p = w[i-nk];
    // char *q = *(temp);
    //  TODO: why this is not possible? pass fixed sized char array by
    //  reference?
    // words[i] = xor_encrypt_2_byte_arrays(words[i - nk], temp, AES_WORD_SIZE);
    // xor_2_words(w[i - nk], temp, w[i], AES_WORD_SIZE);
    i++;
  }
}

void aes_rotate_word(unsigned char *word, size_t len) {
  unsigned char first_byte = word[0];
  for (size_t i = 0; i < len - 1; i++) {
    word[i] = word[i + 1];
  }
  word[len-1] = first_byte;
  return;
}
