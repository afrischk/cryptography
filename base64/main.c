#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base64.h"

int main()
{
    const char* s1 = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    const char* r1 = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
    const char* s2 = "49276d20";
    const char* r2 = "SSdtIA==";
    const char* s3 = "49276d206b";
    const char* r3 = "SSdtIGs=";

    char* enc = b64_encode(s1);
    printf("Test 1 encoded string needs no padding.\n");
    printf("Input:    %s\n", s1);
    printf("Output:   %s\n", enc);
    printf("Expected: %s\n", r1);
    if(strcmp(r1, enc) == 0)
    {
        printf("Test 1 successful!.\n");
    }
    else
    {
        printf("Test 1 failed!. \n");
        return 1;
    }
    free(enc);

    printf("\n");
    enc = b64_encode(s2);
    printf("Test 2 encoded string needs a 2 byte padding.\n");
    printf("Input:    %s\n", s2);
    printf("Output:   %s\n", enc);
    printf("Expected: %s\n", r2);
    if(strcmp(r2, enc) == 0)
    {
        printf("Test 2 successful!.\n");
    }
    else
    {
        printf("Test 2 failed!. \n");
        return 1;
    }
    free(enc);

    printf("\n");
    enc = b64_encode(s3);
    printf("Test 3 encoded string needs a 1 byte padding.\n");
    printf("Input:    %s\n", s3);
    printf("Output:   %s\n", enc);
    printf("Expected: %s\n", r3);
    if(strcmp(r3, enc) == 0)
    {
        printf("Test 3 successful!.\n");
    }
    else
    {
        printf("Test 3 failed!. \n");
        return 1;
    }
    free(enc);
    return 0;
}
