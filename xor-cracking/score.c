#include <stdlib.h>
#include <stdbool.h>
#include "score.h"

/*
 * Checks if the given char is a punctuation char
 * #############################################
 *
 * char c: The char to check.
 * returns: true if the char is a punctuation char
 *
 */
static bool is_punctuation_char(char c)
{
    // space!"',.:;?
    return (c == 32 || c == 33 || c == 34 || c == 39 || c == 44
            || c == 46 || c == 58 || c == 59 || c == 63);
}

/*
 * Scores the given string by summing up the english
 * letter probabilities. It also takes the length of
 * the decrypted text into account that should be
 * close to the encrypted hex string divided by 2.
 * The score is used to validate if the decrypted
 * text is english language.
 * ###############################################
 *
 * const char* text: The decrypted text.
 * size_t len: The length of the text.
 * returns: The score of the decrypted text.
 *
 */
float score_text(const char* text, size_t len)
{
    float score = 0.0;
    size_t text_len = 0;
    for(size_t i = 0; i < len; i++)
    {
        // a-z 97 - 122
        // A-Z 65 - 90
        if(text[i] >= 97 && text[i] <= 122)
        {
            score += LETTER_PROB[text[i] - 97][0];
            text_len++;
        }
        else if(text[i] >= 65 && text[i] <= 90)
        {
            score += LETTER_PROB[text[i] - 65][0];
            text_len++;
        }
        else if(is_punctuation_char(text[i]))
        {
            text_len++;
        }
    }

    // take the length of the decrypted text into
    // account.
    return score * ((float)text_len/((float)len));
}
