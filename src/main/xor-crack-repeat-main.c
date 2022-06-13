#include <stdio.h>
#include <stdlib.h>
#include "hamming.h"
#include "base64.h"
#include "io.h"
#include "score.h"
#include "xor-crack-repeat.h"
#include "xor-crack-types.h"

int main(void)
{
    const char* s1 = "this is a test";
    const char* s2 = "wokka wokka!!!";
    unsigned int distance = hamming_distance(s1, s2);
    printf("Hamming distance is: %d\n", distance);
    const char *str = "wokka wokka";
    float score = score_text(str, 11);
    printf("Score is %f\n", score);
    printf("Start cracking...\n");
    data_t* data = malloc(sizeof(data_t));
    // read from stdin
    data = read(); // TODO warning about prototyping...
    // decode
    data_t* dec_data = b64_decode(data);
    //for(int i = 0; i < dec_data->size; i++){
    //  printf("%c", dec_data->buf[i]);
    //}
    //free(data->buf);
    xor_key_list_t* key_list = xor_get_list_of_scored_key_sizes(dec_data, 2, 41);
    //size_t i = 0;
    //printf("List size is %zu\n", key_list->size);
    //xor_key_node_t* current_node = key_list->first;
    //while(current_node != NULL && i < 11){
        //printf("Score is %f with key size %zu\n", current_node->score, current_node->key_size);
    //    current_node = current_node->next;
    //   i++;
    //}
    xor_crack_key(dec_data, key_list);
    free(data);
    free(key_list);
    free(dec_data);
    return 0;
}
