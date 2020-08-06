#include <stdio.h>
#include <stdlib.h>
#include "hamming.h"
#include "base64.h"

int main(void)
{
    const char* s1 = "this is a test";
    const char* s2 = "wokka wokka!!!";
    unsigned int distance = hamming_distance(s1, s2);
    printf("Hamming distance is: %d\n", distance);

    const char* in_file_name = "../files/xor-crack-repeat.b64";
    const char* out_file_name = "../files/xor-crack-repeat.bin";
    b64_decode_file(in_file_name, out_file_name);

    return 0;
}
