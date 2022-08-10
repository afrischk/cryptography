#include "score.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Checks if the given char is a punctuation char
 * #############################################
 *
 * char c: The char to check.
 * returns: true if the char is a punctuation char
 *
 */

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
struct char_freq *char_occurences(const char *str, size_t len) {
  struct char_freq *freq = malloc(sizeof(struct char_freq));
  freq->occurrences = malloc(sizeof(unsigned int) * 256);
  freq->char_in_str = malloc(sizeof(bool) * 256);
  freq->char_in_str_len = 0;
  freq->total = 0;
  //  init
  for (size_t i = 0; i < 256; i++) {
    freq->occurrences[i] = 0;
    freq->char_in_str[i] = false;
  }

  for (size_t i = 0; i < len; i++) {
    freq->occurrences[(unsigned char)str[i]] += 1;
    freq->char_in_str[(unsigned char)str[i]] = true;
    freq->char_in_str_len++;
    freq->total++;
  }

  return freq;
}

float get_char_freq(unsigned char c) {
  // a-z 97 - 122
  // A-Z 65 - 90
  if (is_small_letter(c)) {
    return LETTER_PROB[c - 97][0];
  } else if (is_capital_letter(c)) {
    return LETTER_PROB[c - 65][0];
  } else if (is_space(c)) {
    return LETTER_PROB[26][0];
  } else {
    return 0.0;
  }
}

float score_text(const char *text, size_t len) {
  struct char_freq *freq = char_occurences(text, len);
  float sum = 0.0;
  for (size_t i = 0; i < 256; i++) {
    if (freq->char_in_str[i]) {
      float char_freq = get_char_freq((unsigned char)i);
      float sqrt = sqrtf(char_freq * ((float)freq->occurrences[(unsigned char)i] /
                                      (float)freq->total));
      sum += sqrt;
    }
  }
  free(freq->occurrences);
  freq->occurrences = NULL;
  free(freq->char_in_str);
  freq->char_in_str = NULL;
  free(freq);
  freq = NULL;
  return sum;
}
