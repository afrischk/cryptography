#include <stdio.h>
#include <stdlib.h>
#include "xor-cracking.h"

int main()
{
    const char* enc = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    char* dec = xor_crack(enc);
    printf("The decrypted message is: %s\n", dec);
    free(dec);
    return 0;
}
