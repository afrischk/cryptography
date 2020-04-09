#ifndef HAMMING_H
#define HAMMING_H

#include <string.h>

/*
 * Calculates hamming distance between 2 (hex)
 * strings.
 * ###########################################
 *
 *  const char* hex1: pointer to the hex string.
 *  const char* hex2: pointer to the hex string.
 *  returns: hamming distance
 */
static inline unsigned int hamming_distance(const char* hex1, const char* hex2)
{
    unsigned int distance = 0;

    if(strlen(hex1) != strlen(hex2))
    {
        return NULL;
    }

    // works for strings of equal length
    while(*hex1)
    {
        unsigned int xor_byte = *hex1++ ^ *hex2++;
        while (xor_byte > 0)
        {
            distance += xor_byte & 1;
            xor_byte >>= 1;
        }
    }

    return distance;
}

#endif /*HAMMING_H*/