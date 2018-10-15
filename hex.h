#ifndef HEX_H
#define HEX_H

#include <stdbool.h>

/*
 * Converts the hex char into an int.
 * 48 - 57 -> 0-9
 * 65 - 90 -> A-Z
 * 97 - 122 -> a-z
 * ##################################
 *
 * char hex: hex char
 * returns: resulting int value
 */
static inline int hex_to_int(char hex)
{
    if(hex <= 90 && hex >= 65) return 10 + hex - 'A';
    if(hex <= 122 && hex >= 97) return 10 + hex - 'a';
    if(hex <= 57 && hex >= 48) return hex - '0';
    return 0;
}

static inline char int_to_hex(int val)
{
    if(val <= 9 && val >= 0) return val + '0';
    if(val <= 15 && val >= 10) return (val - 10)  + 'a';
    return 0;
}

/*
 * Converts two hex chars into one byte (char).
 * ###########################################
 *
 *  const char* hex: pointer to the hex string.
 *  returns: converted char.
 */
static inline char hex_to_byte(const char* hex)
{
    return hex_to_int(hex[0]) << 4 | hex_to_int(hex[1]);
}

/*
 * Converts a char/byte value into a hex char.
 * #########################################
 *
 * char* byte: The byte to convert.
 * bool high_bits: true --> take the 4 highest bits
 *                 false --> takte the 4 lowest bits
 * returns: the converted hex char.
 */
static inline char byte_to_hex(char* byte, bool high_bits)
{
    if(high_bits) return int_to_hex((*byte) >> 4);
    return int_to_hex((*byte) & 0x0f);
}

#endif /*HEX_H*/
