#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xor-encrypt.h"

int main()
{
    printf("Test XOR encoding.\n");

    const char* hex1 = "1c0111001f010100061a024b53535009181c";
    const char* hex2 = "686974207468652062756c6c277320657965";

    printf("Hex string 1: %s\n", hex1);
    printf("XOR against\n");
    printf("Hex string 2: %s\n", hex2);

    char* enc = xor_encrypt(hex1, hex2);

    printf("XOR encoded: %s\n", enc);

    if(strcmp(enc, "746865206b696420646f6e277420706c6179") == 0)
    {
        printf("Test successful!\n");
    }

    free(enc);
    return 0;
}
