#ifndef AES_H
#define AES_H

#define AES_BLOCK_LEN 128 // 16 byte or 128 bit
#define AES_NB 4 // 16/4 byte or 128/32 bit; columns of state

// aes 128 specific
#define AES_128_KEY_LEN 128 // 16 byte or 128 bit
#define AES_128_NUM_OF_ROUNDS 10

typedef unsigned char word[4];

void aes_sub_bytes(void);
void aes_shift_rows(void);
void aes_mix_columns(void);
void aes_add_round_key(void);
void aes_expand_key(void);
void aes_rotate_word(word *w);
unsigned char aes_round_constant(unsigned char i);
void aes_encrypt(void);
void aes_decrypt(void);

#endif /*AES_H*/
