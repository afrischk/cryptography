#ifndef XOR_CRACK_TYPES_H
#define XOR_CRACK_TYPES_H

#include <stddef.h>

typedef struct xor_crk_res
{
    char *dec_res;
    float score;
    char key;
    size_t len;
} xor_crk_res_t;

typedef struct xor_key_node
{
    struct xor_key_node* next;
    struct xor_key_node* prev;
    float score;
    size_t key_size;
} xor_key_node_t;

typedef struct xor_key_list
{
    size_t size;
    xor_key_node_t* first;

} xor_key_list_t;

#endif /*XOR_CRACK_TYPES_H*/
