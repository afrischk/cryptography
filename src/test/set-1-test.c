#include "aes_galois.h"
#include "algorithms.h"
#include "base64.h"
#include "io.h"
#include "unity_internals.h"
#include "xor.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unity.h>

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void hamming_distance_tests(void) {
  printf("\n");
  printf("###############################\n");
  printf("#### Hamming Distance Tests ###\n");
  printf("###############################\n");
  printf("\n");

  const char *s1 = "this is a test";
  const char *s2 = "wokka wokka!!!";
  printf("Calc Hamming Distance between: %s <-> %s\n", s1, s2);
  unsigned int distance = alg_hamming_dist(s1, s2);
  printf("Hamming distance is: %d\n", distance);
  printf("Excpected: %d\n", 37);
  TEST_ASSERT_EQUAL_UINT(37, distance);
}

void base64_tests(void) {
  printf("\n");
  printf("###############################\n");
  printf("######## Base64 Tests #########\n");
  printf("###############################\n");
  printf("\n");

  const char *s1 = "49276d206b696c6c696e6720796f757220627261696e206c696b6520612"
                   "0706f69736f6e6f7573206d757368726f6f6d";
  const char *r1 =
      "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
  const char *s2 = "49276d20";
  const char *r2 = "SSdtIA==";
  const char *s3 = "49276d206b";
  const char *r3 = "SSdtIGs=";
  const char *s4 = "I'm killing your brain like a poisonous mushroom";

  char *enc = b64_encode(s1);
  printf("Test 1 encoded string needs no padding.\n");
  printf("Input:    %s\n", s1);
  printf("Output:   %s\n", enc);
  printf("Expected: %s\n", r1);
  TEST_ASSERT(strcmp(r1, enc) == 0);
  free(enc);

  printf("\n");
  enc = b64_encode(s2);
  printf("Test 2 encoded string needs a 2 byte padding.\n");
  printf("Input:    %s\n", s2);
  printf("Output:   %s\n", enc);
  printf("Expected: %s\n", r2);
  TEST_ASSERT(strcmp(r2, enc) == 0);
  free(enc);

  printf("\n");
  enc = b64_encode(s3);
  printf("Test 3 encoded string needs a 1 byte padding.\n");
  printf("Input:    %s\n", s3);
  printf("Output:   %s\n", enc);
  printf("Expected: %s\n", r3);
  TEST_ASSERT(strcmp(r3, enc) == 0);
  free(enc);

  printf("\n");
  printf("Base64 decode!\n");
  struct io_data *input = malloc(sizeof(struct io_data));
  input->buf = (char *)r1;
  input->size = strlen(r1);
  struct io_data *res = b64_decode(input);
  printf("Input:    %s\n", r1);
  printf("Output:   %s\n", res->buf);
  printf("Expected: %s\n", s4);
  TEST_ASSERT(strcmp(s4, res->buf) == 0);
  free(input);
  free(res->buf);
  free(res);
}

void xor_tests(void) {
  printf("\n");
  printf("###############################\n");
  printf("########## XOR Tests ##########\n");
  printf("###############################\n");
  printf("\n");

  printf("Test 1:\n");
  const char *hex1 = "1c0111001f010100061a024b53535009181c";
  const char *hex2 = "686974207468652062756c6c277320657965";
  printf("Hex string 1: %s\n", hex1);
  printf("              ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  printf("Hex string 2: %s\n", hex2);
  char *enc1 = xor_encrypt_2_hex_str(hex1, hex2);
  printf("XOR encoded:  %s\n", enc1);
  TEST_ASSERT(strcmp(enc1, "746865206b696420646f6e277420706c6179") == 0);
  free(enc1);

  printf("\n");
  printf("Test 2:\n");
  const char *enc2 =
      "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
  struct xor_data *res = xor_crack_hex_str(enc2);
  printf("The decrypted message is: %s\n", res->dec);
  printf("With score: %f\n", res->score);
  printf("And key: %c\n", res->key);
  TEST_ASSERT(res->key == 'X');
  free(res->dec);
  free(res);

  printf("\n");
  printf("Test 3:\n");
  struct xor_data *res2 = xor_crack_with_most_likely_1_byte_key(
      "../../files/base-set-1/xor-encrypted-hex.txt");
  printf("Plaintext: %s", res2->dec);
  printf("Key: %c\n", res2->key);
  printf("Score: %f\n", res2->score);
  TEST_ASSERT(res2->key == '5');
  free(res2->dec);
  free(res2);

  printf("\n");
  printf("Test 4:\n");
  printf("Use ../../files/base-set-1/xor-plain-text.txt to encrypt\n");
  struct io_data *data = read("../../files/base-set-1/xor-plain-text.txt");
  const char *res3 = xor_encrypt_with_n_bytes_key_to_hex_str(data, "ICE");
  printf("Encrypted data in hex is %s\n", res3);
  TEST_ASSERT(strcmp(res3,
                     "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d6"
                     "3343c2a26226324272765272a282b2f20430a652e2c652a3124"
                     "333a653e2b2027630c692b20283165286326302e27282f") == 0);
  free((void *)res3);
  free(data->buf);
  free(data);
}

void aes_tests() {
  printf("\n");
  printf("###############################\n");
  printf("########## AES Tests ##########\n");
  printf("###############################\n");
  printf("\n");

  printf("Test 1:\n");
  unsigned char a = 0x57;
  unsigned char b = 0x83;
  unsigned char p = aes_gmul(a, b);
  printf("Gmul result is: %x\n", p);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(hamming_distance_tests);
  RUN_TEST(base64_tests);
  RUN_TEST(xor_tests);
  RUN_TEST(aes_tests);
  return UNITY_END();
}
