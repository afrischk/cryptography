#ifndef SCORE_H
#define SCORE_H

#include <stdbool.h>
#include <stddef.h>

static float LETTER_PROB[27][1] =
{
    /*a*/{0.0651738}, /*b*/{0.0124248}, /*c*/{0.0217339}, /*d*/{0.0349835},
    /*e*/{0.1041442}, /*f*/{0.0197881}, /*g*/{0.0158610}, /*h*/{0.0492888},
    /*i*/{0.0558094}, /*j*/{0.0009033}, /*k*/{0.0050529}, /*l*/{0.0331490},
    /*m*/{0.0202124}, /*n*/{0.0564513}, /*o*/{0.0596302}, /*p*/{0.0137645},
    /*q*/{0.0008606}, /*r*/{0.0497563}, /*s*/{0.0515760}, /*t*/{0.0729357},
    /*u*/{0.0225134}, /*v*/{0.0082903}, /*w*/{0.0171272}, /*x*/{0.0013692},
    /*y*/{0.0145984}, /*z*/{0.0007836}, /* */{0.1918182}
};

struct char_freq {
  unsigned int *occurrences;
  bool *char_in_str;
  size_t char_in_str_len;
  size_t total;
};

float score_text(const char* text, size_t len);

static inline bool is_small_letter(char c) { return c >= 97 && c <= 122; }
static inline bool is_capital_letter(char c) { return c >= 65 && c <= 90; }
static inline bool is_space(char c) { return c == 32; }

static inline bool is_text(char c) {
  return is_small_letter(c) || is_capital_letter(c) || is_space(c);
}

static inline bool is_punctuation_char(char c) {
  // space!"',.:;?
  return (c == 32 || c == 33 || c == 34 || c == 39 || c == 44 || c == 46 ||
          c == 58 || c == 59 || c == 63);
}


#endif /*SCORE_H*/
