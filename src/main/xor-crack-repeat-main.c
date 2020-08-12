#include <stdio.h>
#include <stdlib.h>
#include "hamming.h"
#include "base64.h"
#include "xor-crack-repeat.h"
#include "xor-crack-types.h"

int main(void)
{
    /*const char* s1 = "this is a test";
    const char* s2 = "wokka wokka!!!";
    unsigned int distance = hamming_distance(s1, s2);
    printf("Hamming distance is: %d\n", distance);

    const char* in_file_name = "../files/xor-crack-repeat.b64";
    const char* out_file_name = "../files/xor-crack-repeat.bin";
    b64_decode_file(in_file_name, out_file_name);*/

    const char* in_file_name = "../files/xor-crack-repeat.b64";
    const char* out_file_name = "../files/xor-crack-repeat.bin";
    b64_decode_file(in_file_name, out_file_name);
    xor_key_list_t* key_list = xor_get_list_of_scored_key_sizes(out_file_name, 2, 40);

    //printf("List size is %zu\n", key_list->size);
    xor_key_node_t* current_node = key_list->first;
    while(current_node != NULL)
    {
        //printf("Score is %f with key size %zu\n", current_node->score, current_node->key_size);
        current_node = current_node->next;
    }

    xor_crack_key(out_file_name, key_list);

    //free_key_list(key_list);
    return 0;
}
