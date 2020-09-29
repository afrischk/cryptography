#ifndef XOR_CRACK_REPEAT_H
#define XOR_CRACK_REPEAT_H
#include "xor-crack-types.h"

xor_key_list_t* xor_get_list_of_scored_key_sizes(const char* byte_file, int min_key_size, int max_key_size);
char* xor_crack_key(const char* byte_file, xor_key_list_t* list);
void free_key_list(xor_key_list_t* list);

#endif /*XOR_CRACK_REPEAT_H*/
