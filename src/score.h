#ifndef SCORE_H
#define SCORE_H

#include <stdbool.h>
#include <stddef.h>

// english letter probabilites
// TODO seems to be wrong?!
//static float LETTER_PROB[26][1] =
//{
//    /*a*/{0.08167}, /*b*/{0.01492}, /*c*/{0.02782}, /*d*/{0.04253},
//    /*e*/{0.12702}, /*f*/{0.02228}, /*g*/{0.02015}, /*h*/{0.06094},
//    /*i*/{0.06966}, /*j*/{0.00153}, /*k*/{0.00772}, /*l*/{0.04025},
//    /*m*/{0.02406}, /*n*/{0.06749}, /*o*/{0.07507}, /*p*/{0.01929},
//    /*q*/{0.00095}, /*r*/{0.05987}, /*s*/{0.06327}, /*t*/{0.09056},
//    /*u*/{0.02758}, /*v*/{0.00978}, /*w*/{0.02360}, /*x*/{0.00150},
//    /*y*/{0.01974}, /*z*/{0.00074}
//};

//static float LETTER_PROB[26][1] =
//{
//    /*a*/{0.0855}, /*b*/{0.0160}, /*c*/{0.0316}, /*d*/{0.0387},
//    /*e*/{0.1210}, /*f*/{0.0218}, /*g*/{0.0209}, /*h*/{0.0496},
//    /*i*/{0.0733}, /*j*/{0.0022}, /*k*/{0.0081}, /*l*/{0.0421},
//    /*m*/{0.0253}, /*n*/{0.0717}, /*o*/{0.0747}, /*p*/{0.0207},
//    /*q*/{0.0001}, /*r*/{0.0633}, /*s*/{0.0673}, /*t*/{0.0894},
//    /*u*/{0.0268}, /*v*/{0.0106}, /*w*/{0.0183}, /*x*/{0.0019},
//    /*y*/{0.0172}, /*z*/{0.0011}
//};

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

float score_text(const char* text, size_t len);

#endif /*SCORE_H*/
