#ifndef XOR_CRACKING_H
#define XOR_CRACKING_H

extern char* xor_crack(const char* enc_hex);
extern char* xor_decrypt(const char* enc_hex, char key, size_t len);

#endif /*XOR_CRACKING_H*/
