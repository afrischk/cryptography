#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <float.h>
#include "hamming.h"
#include "xor-crack-types.h"

extern int errno;

static xor_key_node_t *init_xor_key_node()
{
    xor_key_node_t *node = malloc(sizeof(xor_key_node_t));
    node->key_size = 0;
    node->score = FLT_MAX;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

static xor_key_list_t *init_xor_key_list()
{
    xor_key_list_t *list = malloc(sizeof(xor_key_list_t));
    list->size = 0;
    return list;
}

static xor_key_list_t* xor_add_node(xor_key_list_t *list, xor_key_node_t *new_node)
{
    for (xor_key_node_t* current_node = list->first; current_node != NULL; current_node = current_node->next)
    {
        if (new_node->score >= current_node->score)
        {
            // add to end of list
            if (current_node->next == NULL)
            {
                current_node->next = new_node;
                new_node->prev = current_node;
                // end loop
                break;
            }
        }
        else
        {
            new_node->next = current_node;
            new_node->prev = current_node->prev;
            current_node->prev = new_node;
            if (new_node->prev == NULL)
            {
                // if no prev available its the first node
                list->first = new_node;
            }
            else
            {
                new_node->prev->next = new_node;
            }

            // end loop
            break;
        }
    }

    list->size++;
    return list;
}

// returns a sorted list (scores) of key sizes
static xor_key_list_t* xor_add_key_to_list(xor_key_list_t *list, size_t key_size, float score)
{
    xor_key_node_t *new_node = init_xor_key_node();
    new_node->key_size = key_size;
    new_node->score = score;

    // empty list
    if (list->size == 0)
    {
        list->first = new_node;
        list->size++;
        return list;
    }

    return xor_add_node(list, new_node);
}

xor_key_list_t *xor_det_key(const char *byte_file, size_t min_key_size, size_t max_key_size)
{
    FILE *in = fopen(byte_file, "rb");
    if (in == NULL)
    {
        fprintf(stderr, "Error occured: %d Could not open file: %s\n", errno, strerror(errno));
    }
    char *buffer = malloc(2 * max_key_size * sizeof(char));
    // read max_key_size worth of bytes
    // its all we need to calculate the key
    size_t bytes_read = fread(buffer, 1, 2 * max_key_size, in);
    if (bytes_read < 2 * max_key_size)
    {
        fprintf(stderr, "Not enough bytes in file. Took %zu bytes.\n", bytes_read);
    }

    xor_key_list_t *key_list = init_xor_key_list();

    for (size_t key_size = min_key_size; key_size < bytes_read; key_size++)
    {
        char *key1 = malloc(key_size * sizeof(char));
        char *key2 = malloc(key_size * sizeof(char));

        strncpy(key1, buffer, key_size);
        strncpy(key2, buffer + key_size, key_size);

        unsigned int distance = hamming_distance(key1, key2);
        float score = (float)distance / (float)key_size;

        key_list = xor_add_key_to_list(key_list, key_size, score);

        free(key1);
        free(key2);
    }

    free(buffer);
    return key_list;
}