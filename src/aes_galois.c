#include "aes_galois.h"

unsigned char aes_gmul(unsigned char a, unsigned char b) {
  unsigned char p = 0;
  while (a != 0 && b != 0) {
    if (b & 1) {
      p ^= a; // b0 is 1
    }
    if (a & 0x80) { // is b7 1? then reduce
      a = (a << 1) ^ 0x11b; // subtract x^8 + x^4 + x^3 + x + 1
    } else {
      a <<= 1; // a*x
    }
    b >>= 1; // shift right to get the next bit
  }
  return p;
}
