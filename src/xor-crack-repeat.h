#ifndef XOR_CRACK_REPEAT_H
#define XOR_CRACK_REPEAT_H
#include "xor-crack-types.h"
#include "algorithms.h"
#include "io.h"

struct alg_list* xor_get_list_of_scored_key_sizes(const struct io_data* data, size_t min_key_size, size_t max_key_size);
char* xor_crack_key(struct io_data *data, struct alg_list* list);
void free_key_list(struct alg_list* list);

#endif /*XOR_CRACK_REPEAT_H*/
