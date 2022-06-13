#ifndef XOR_CRACK_REPEAT_H
#define XOR_CRACK_REPEAT_H
#include "xor-crack-types.h"
#include "io.h"

xor_key_list_t* xor_get_list_of_scored_key_sizes(const data_t* data, size_t min_key_size, size_t max_key_size);
char* xor_crack_key(data_t *data, xor_key_list_t* list);
void free_key_list(xor_key_list_t* list);

#endif /*XOR_CRACK_REPEAT_H*/
