#ifndef AES_H
#define AES_H

#include <stddef.h>
#define AES_BLOCK_SIZE 4 // Nb words
#define AES_WORD_SIZE 4

// aes 128 specific
#define AES_128_KEY_LEN 4 // Nk words
#define AES_128_NUM_OF_ROUNDS 10 // Nr

//typedef char word[AES_WORD_SIZE];

void aes_sub_bytes(void);
void aes_shift_rows(void);
void aes_mix_columns(void);
void aes_add_round_key(void);
void aes_expand_key(char const* key, char** words, size_t nk, size_t nr);
void aes_rotate_word(unsigned char* word, size_t len);
void aes_sub_word(char* word);
unsigned char aes_round_constant(unsigned char i);
void aes_encrypt(void);
void aes_decrypt(void);

#endif /*AES_H*/
