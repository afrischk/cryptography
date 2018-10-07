#ifndef HEX_H
#define HEX_H

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

#endif /*HEX_H*/
