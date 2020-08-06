#ifndef BASE64_H
#define BASE64_H

#include <stdbool.h>

char *b64_encode(const char* hex);
char *b64_decode(const char* b64, size_t buf_size);
void b64_decode_file(const char* file_name_in, const char* file_name_out);
void b64_expand_bytes(const char* hex, char* enc, int* pos, int pad);
void b64_collapse_bytes(const char* b64, int pad, bool last_4_bytes, char* dec, int* pos);

static const unsigned char B64_TABLE[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};

/*
 * Converts the b64 char into the encoded byte.
 * A-Z 65 - 90
 * a-z 97 - 122
 * 0-9 48 -57
 * + 43
 * / 47
 * ##################################
 *
 * char* b64: b64 char
 * returns: resulting byte value
 */
static inline unsigned int b64_to_enc_byte(const char* b64)
{

    if(*b64 >= 65 && *b64 <= 90) return *b64 - 'A';
    if(*b64 >= 97 && *b64 <= 122) return *b64 - 'a' + 26; // add the offset -> B64_TABLE
    if(*b64 >= 48 && *b64 <= 57) return *b64 - '0' + 52; // add the offset -> B64_TABLE
    if(*b64 == 43) return 62;
    if(*b64 == 47) return 63;
    // error --> =
    return 66;
}

#endif /*BASE64_H*/
