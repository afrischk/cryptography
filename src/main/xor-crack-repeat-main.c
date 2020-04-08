#include <stdio.h>
#include <stdlib.h>
#include "hamming.h"

int main()
{
    const char* s1 = "this is a test";
    const char* s2 = "wokka wokka!!!";
    unsigned int distance = hamming_distance(s1, s2);
    printf("Hamming distance is: %d\n", distance);
    return 0;
}
