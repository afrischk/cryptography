#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <float.h>
#include "hamming.h"
#include "xor-crack-types.h"
#include "xor-crack.h"

extern int errno;

static xor_key_node_t* xor_init_key_node()
{
    xor_key_node_t *node = malloc(sizeof(xor_key_node_t));
    node->key_size = 0;
    node->score = FLT_MAX;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

static xor_key_list_t* init_key_List()
{
    xor_key_list_t* list = malloc(sizeof(xor_key_list_t));
    list->size = 0;
    return list;
}

static void add_key_node_as_last(xor_key_node_t *node, xor_key_node_t *new)
{
    node->next = new;
    new->prev = node;
}

static void add_key_node_sorted(xor_key_list_t* list, xor_key_node_t* node, xor_key_node_t* new)
{
    new->next = node;
    new->prev = node->prev;
    node->prev = new;
    if (new->prev == NULL)
    {
        // if no prev available its the first node
        list->first = new;
    }
    else
    {
        new->prev->next = new;
    }
}

static xor_key_list_t *add_key_node(xor_key_list_t *list, xor_key_node_t *new_node)
{
    for (xor_key_node_t *current_node = list->first; current_node != NULL; current_node = current_node->next)
    {
        if (new_node->score >= current_node->score)
        {
            // add to end of list
            if (current_node->next == NULL)
            {
                add_key_node_as_last(current_node, new_node);
                break;
            }
        }
        else
        {
            add_key_node_sorted(list, current_node, new_node);
            break;
        }
    }

    list->size++;
    return list;
}

// returns a sorted list (scores) of key sizes
static xor_key_list_t *add_key_size_sorted(xor_key_list_t *list, size_t key_size, float score)
{
    xor_key_node_t *new_node = xor_init_key_node();
    new_node->key_size = key_size;
    new_node->score = score;

    // empty list
    if (list->size == 0)
    {
        list->first = new_node;
        list->size++;
        return list;
    }

    return add_key_node(list, new_node);
}

// buggy
void free_key_list(xor_key_list_t* list)
{
    xor_key_node_t* node = list->first;
    while(node->next != NULL)
    {
        xor_key_node_t* next = node->next;
        free(node);
        node = next;
    }
    free(node);
}

xor_key_list_t* xor_get_list_of_scored_key_sizes(const char* byte_file, size_t min_key_size, size_t max_key_size)
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

    xor_key_list_t *key_list = init_key_List();
    for (size_t key_size = min_key_size; key_size < bytes_read; key_size++)
    {
        char *key1 = malloc(key_size * sizeof(char));
        char *key2 = malloc(key_size * sizeof(char));

        strncpy(key1, buffer, key_size);
        strncpy(key2, buffer + key_size, key_size);

        unsigned int distance = hamming_distance(key1, key2);
        float score = (float)distance / (float)key_size;

        key_list = add_key_size_sorted(key_list, key_size, score);

        free(key1);
        free(key2);
    }

    free(buffer);
    fclose(in);
    return key_list;
}

char* xor_crack_key(const char* byte_file, xor_key_list_t* list)
{
    FILE *in = fopen(byte_file, "rb");
    if (in == NULL)
    {
        fprintf(stderr, "Error occured: %d Could not open file: %s\n", errno, strerror(errno));
    }

    fseek(in, 0, SEEK_END);
    size_t fsize = ftell(in);
    fseek(in, 0, SEEK_SET);

    char* bytes = malloc(fsize * sizeof(char));
    fread(bytes, 1, fsize, in);

    size_t key_size = list->first->key_size;
    printf("Start cracking with eky size %zu\n", key_size);
    //size_t block_len = (size_t)fsize/key_size; // ignore rest
    //char* block = malloc(block_len * sizeof(char));
    for(size_t i = 0; i < key_size; i++)
    {
        xor_crack_bytes(bytes, fsize, i, key_size);
    }

    //free(block);
    free(bytes);
    fclose(in);
    return "";
}
