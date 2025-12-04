// system includes
#include <algorithm>
#include <array>
#include <cstring>
#include <string>
#include <vector>

// Google Test includes
#include <gtest/gtest.h>

// project includes
#include "sha256/sha256.h"
#include "types/types.h"

struct TestVector {
  std::string name;
  std::string input;
  std::string expected_hex;
};

// Standard NIST Test Vectors
const std::vector<TestVector> NIST_VECTORS = {
    {"Empty String", "",
     "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"},
    {"String 'abc'", "abc",
     "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"},
    {"String 'abcdbcdec...'",
     "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
     "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1"}};

// Convert raw bytes to hex string for comparison
std::string bytes_to_hex_string(const uint8_t *data, size_t len) {
  static const char hexmap[] = "0123456789abcdef";
  std::string s(len * 2, ' ');
  for (size_t i = 0; i < len; ++i) {
    s[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
    s[2 * i + 1] = hexmap[data[i] & 0x0F];
  }
  return s;
}

TEST(SHA256_OneShot, Hex_StandardVectors) {
  char output[SHA256::SHA256_HEX_SIZE];

  for (const auto &tv : NIST_VECTORS) {
    // Dirt-ify buffer to ensure null-terminator is actually written
    memset(output, 'X', SHA256::SHA256_HEX_SIZE);

    SHA256::sha256_hex(tv.input.data(), tv.input.size(), output);

    EXPECT_STREQ(output, tv.expected_hex.c_str()) << "Failed on: " << tv.name;
    EXPECT_EQ(output[64], '\0') << "Failed to null terminate: " << tv.name;
  }
}

TEST(SHA256_OneShot, Bytes_StandardVectors) {
  uint8_t output[SHA256::SHA256_BYTES_SIZE];

  for (const auto &tv : NIST_VECTORS) {
    SHA256::sha256_bytes(tv.input.data(), tv.input.size(), output);

    std::string hex_result =
        bytes_to_hex_string(output, SHA256::SHA256_BYTES_SIZE);
    EXPECT_EQ(hex_result, tv.expected_hex) << "Failed on: " << tv.name;
  }
}

TEST(SHA256_Streaming, Fragmentation_Consistency) {
  // Ensure hashing "abc" in one go is same as "a" + "b" + "c"
  SHA256::sha256 ctx;
  char output[SHA256::SHA256_HEX_SIZE];

  SHA256::sha256_init(&ctx);
  SHA256::sha256_append(&ctx, "a", 1);
  SHA256::sha256_append(&ctx, "b", 1);
  SHA256::sha256_append(&ctx, "c", 1);
  SHA256::sha256_finalize_hex(&ctx, output);

  EXPECT_STREQ(
      output,
      "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
}

TEST(SHA256_Streaming, ContextReuse) {
  // Ensure calling init() on a dirty struct resets it correctly
  SHA256::sha256 ctx;
  char output[SHA256::SHA256_HEX_SIZE];

  // First usage
  SHA256::sha256_init(&ctx);
  SHA256::sha256_append(&ctx, "abc", 3);
  SHA256::sha256_finalize_hex(&ctx, output);

  // Reuse
  SHA256::sha256_init(&ctx);
  SHA256::sha256_append(&ctx, "", 0); // Empty string
  SHA256::sha256_finalize_hex(&ctx, output);

  // Expect empty string hash
  EXPECT_STREQ(
      output,
      "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
}

TEST(SHA256_Streaming, FinalizeBytes_Works) {
  SHA256::sha256 ctx;
  uint8_t output[SHA256::SHA256_BYTES_SIZE];

  SHA256::sha256_init(&ctx);
  SHA256::sha256_append(&ctx, "abc", 3);
  SHA256::sha256_finalize_bytes(&ctx, output);

  std::string hex = bytes_to_hex_string(output, SHA256::SHA256_BYTES_SIZE);
  EXPECT_EQ(hex,
            "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
}

TEST(SHA256_EdgeCases, BlockBoundary_55Bytes) {
  /* SHA-256 Block size is 64 bytes.
     Padding logic: Append 0x80, then zeros, then 8 bytes of length (Total 64).

     If input is 55 bytes:
     55 data + 1 (0x80) + 8 (length) = 64.
     This fits EXACTLY in one block.
  */
  std::string input(55, 'a');
  char output[SHA256::SHA256_HEX_SIZE];

  SHA256::sha256_hex(input.data(), input.size(), output);

  // Known hash for 55 'a's
  // Verified via `echo -n
  // "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" | sha256sum`
  const char *expected =
      "9f4390f8d30c2dd92ec9f095b65e2b9ae9b0a925a5258e241c9f1e910f734318";
  EXPECT_STREQ(output, expected);
}

TEST(SHA256_EdgeCases, BlockBoundary_56Bytes_Spillover) {
  /* If input is 56 bytes:
     56 data + 1 (0x80) = 57.
     We need 8 bytes for length. 57 + 8 = 65.
     This EXCEEDS 64. This forces the algorithm to create a second block.
     This tests the `while (sha->buffer_counter != 56)` loop in your finalize
     function.
  */
  std::string input(56, 'a');
  char output[SHA256::SHA256_HEX_SIZE];

  SHA256::sha256_hex(input.data(), input.size(), output);

  // Verified via sha256sum
  const char *expected =
      "b35439a4ac6f0948b6d6f9e3c6af0f5f590ce20f1bde7090ef7970686ec6738a";
  EXPECT_STREQ(output, expected);
}

TEST(SHA256_EdgeCases, LongInput_MultiBlock) {
  // 200 bytes ensures we process 3 full blocks (64*3=192) + remainder
  std::string input(200, 'A');
  char output[SHA256::SHA256_HEX_SIZE];

  SHA256::sha256_hex(input.data(), input.size(), output);

  // Verified via sha256sum
  const char *expected =
      "70d3bf8b0b9d83a61012f35fbf460c4207063fe31b4d6178390fe3b721cc03f7";
  EXPECT_STREQ(output, expected);
}

TEST(SHA256_Conversation, HexToBytes) {
  // 10011110 10111010 00111000 10010011 01011000 00100000 00010001 10101000
  // 10011110 00110011 00110000 10001101 10100110 00011001 10100101 11111001
  // 01111001 01011000 00010010 10000101 00100100 11001100 00110011 10101001
  // 10011011 01101000 10011101 00100100 10101101 01100000 00000111 00011010
  std::string input_hex =
      "9eba3893582011a89e33308da619a5f97958128524cc33a99b689d24ad60071a";

  Hash expected_result_A = {
      0b10011110, 0b10111010, 0b00111000, 0b10010011, 0b01011000, 0b00100000,
      0b00010001, 0b10101000, 0b10011110, 0b00110011, 0b00110000, 0b10001101,
      0b10100110, 0b00011001, 0b10100101, 0b11111001, 0b01111001, 0b01011000,
      0b00010010, 0b10000101, 0b00100100, 0b11001100, 0b00110011, 0b10101001,
      0b10011011, 0b01101000, 0b10011101, 0b00100100, 0b10101101, 0b01100000,
      0b00000111, 0b00011010};

  Hash result_A = SHA256::hashStringToArray(input_hex);
  EXPECT_EQ(result_A, expected_result_A);

  // 00000111 10100011 01011000 11010111 01010010 01001010 00001000 11001011
  // 11000110 10000110 00100100 01010111 10100110 00001010 00011110 11000111
  // 10111000 01110110 00000111 01010001 10011100 01011000 00101001 10011011
  // 11000100 10011001 01011001 11010011 00000000 11101010 10111101 01010010
  input_hex =
      "07a358d7524a08cbc6862457a60a1ec7b87607519c58299bc49959d300eabd52";

  Hash expected_result_B = {
      0b00000111, 0b10100011, 0b01011000, 0b11010111, 0b01010010, 0b01001010,
      0b00001000, 0b11001011, 0b11000110, 0b10000110, 0b00100100, 0b01010111,
      0b10100110, 0b00001010, 0b00011110, 0b11000111, 0b10111000, 0b01110110,
      0b00000111, 0b01010001, 0b10011100, 0b01011000, 0b00101001, 0b10011011,
      0b11000100, 0b10011001, 0b01011001, 0b11010011, 0b00000000, 0b11101010,
      0b10111101, 0b01010010};

  Hash result_B = SHA256::hashStringToArray(input_hex);
  EXPECT_EQ(result_B, expected_result_B);
}

TEST(SHA256_Conversation, BytesToHex) {
  Hash input = {0b10011110, 0b10111010, 0b00111000, 0b10010011, 0b01011000,
                0b00100000, 0b00010001, 0b10101000, 0b10011110, 0b00110011,
                0b00110000, 0b10001101, 0b10100110, 0b00011001, 0b10100101,
                0b11111001, 0b01111001, 0b01011000, 0b00010010, 0b10000101,
                0b00100100, 0b11001100, 0b00110011, 0b10101001, 0b10011011,
                0b01101000, 0b10011101, 0b00100100, 0b10101101, 0b01100000,
                0b00000111, 0b00011010};

  std::string expected_hex =
      "9eba3893582011a89e33308da619a5f97958128524cc33a99b689d24ad60071a";

  std::string result_hex = SHA256::hashArrayToString(input);
  EXPECT_EQ(result_hex, expected_hex);
}

// Direct test coverage for sha256_init
TEST(SHA256_Functions, Init_InitializesStateCorrectly) {
  SHA256::sha256 ctx;
  SHA256::sha256_init(&ctx);

  // Verify initial state values match SHA256 constants
  EXPECT_EQ(ctx.state[0], 0x6a09e667);
  EXPECT_EQ(ctx.state[1], 0xbb67ae85);
  EXPECT_EQ(ctx.state[2], 0x3c6ef372);
  EXPECT_EQ(ctx.state[3], 0xa54ff53a);
  EXPECT_EQ(ctx.state[4], 0x510e527f);
  EXPECT_EQ(ctx.state[5], 0x9b05688c);
  EXPECT_EQ(ctx.state[6], 0x1f83d9ab);
  EXPECT_EQ(ctx.state[7], 0x5be0cd19);
  EXPECT_EQ(ctx.n_bits, 0);
  EXPECT_EQ(ctx.buffer_counter, 0);
}

// Direct test coverage for sha256_append
TEST(SHA256_Functions, Append_AccumulatesBits) {
  SHA256::sha256 ctx;
  SHA256::sha256_init(&ctx);

  // Append 5 bytes
  SHA256::sha256_append(&ctx, "hello", 5);
  EXPECT_EQ(ctx.n_bits, 5 * 8); // 40 bits

  // Append 4 more bytes
  SHA256::sha256_append(&ctx, "test", 4);
  EXPECT_EQ(ctx.n_bits, 9 * 8); // 72 bits
}

// Direct test coverage for sha256_finalize_hex
TEST(SHA256_Functions, FinalizeHex_ProducesCorrectOutput) {
  SHA256::sha256 ctx;
  char output[SHA256::SHA256_HEX_SIZE];

  SHA256::sha256_init(&ctx);
  SHA256::sha256_append(&ctx, "test", 4);
  SHA256::sha256_finalize_hex(&ctx, output);

  // Verified via PowerShell and system hash functions
  const char *expected =
      "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08";
  EXPECT_STREQ(output, expected);
  EXPECT_EQ(output[64], '\0');
}

// Direct test coverage for sha256_finalize_bytes
TEST(SHA256_Functions, FinalizeBytes_ProducesCorrectOutput) {
  SHA256::sha256 ctx;
  uint8_t output[SHA256::SHA256_BYTES_SIZE];

  SHA256::sha256_init(&ctx);
  SHA256::sha256_append(&ctx, "test", 4);
  SHA256::sha256_finalize_bytes(&ctx, output);

  // Verified via PowerShell and system hash functions
  std::string hex_result =
      bytes_to_hex_string(output, SHA256::SHA256_BYTES_SIZE);
  const char *expected =
      "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08";
  EXPECT_STREQ(hex_result.c_str(), expected);
}

// Direct test coverage for sha256_hex
TEST(SHA256_Functions, Sha256Hex_ProducesCorrectOutput) {
  char output[SHA256::SHA256_HEX_SIZE];
  SHA256::sha256_hex("test", 4, output);

  const char *expected =
      "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08";
  EXPECT_STREQ(output, expected);
  EXPECT_EQ(output[64], '\0');
}

// Direct test coverage for sha256_bytes
TEST(SHA256_Functions, Sha256Bytes_ProducesCorrectOutput) {
  uint8_t output[SHA256::SHA256_BYTES_SIZE];
  SHA256::sha256_bytes("test", 4, output);

  std::string hex_result =
      bytes_to_hex_string(output, SHA256::SHA256_BYTES_SIZE);
  const char *expected =
      "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08";
  EXPECT_STREQ(hex_result.c_str(), expected);
}

// Direct test coverage for hashStringToArray
TEST(SHA256_Functions, HashStringToArray_ConvertsCorrectly) {
  std::string hex =
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";
  Hash result = SHA256::hashStringToArray(hex);

  // Verify first and last bytes
  EXPECT_EQ(result[0], 0x01);
  EXPECT_EQ(result[31], 0xef);

  // Verify full conversion
  std::string roundtrip = SHA256::hashArrayToString(result);
  EXPECT_EQ(roundtrip, hex);
}

// Direct test coverage for hashArrayToString
TEST(SHA256_Functions, HashArrayToString_ConvertsCorrectly) {
  Hash input;
  input.fill(0x00);
  input[0] = 0xFF;
  input[15] = 0xAB;
  input[31] = 0xCD;

  std::string hex = SHA256::hashArrayToString(input);

  // Verify format
  EXPECT_EQ(hex.length(), 64);
  EXPECT_EQ(hex[0], 'f');
  EXPECT_EQ(hex[1], 'f');
  EXPECT_EQ(hex[30], 'a');
  EXPECT_EQ(hex[31], 'b');
  EXPECT_EQ(hex[62], 'c');
  EXPECT_EQ(hex[63], 'd');
}

// Test hashStringToArray with invalid input
TEST(SHA256_Functions, HashStringToArray_ThrowsOnInvalidLength) {
  std::string short_hex = "0123456789abcdef";
  EXPECT_THROW(SHA256::hashStringToArray(short_hex), std::invalid_argument);

  std::string long_hex(100, '0');
  EXPECT_THROW(SHA256::hashStringToArray(long_hex), std::invalid_argument);
}

// Test empty string handling
TEST(SHA256_Functions, Sha256Hex_EmptyString) {
  char output[SHA256::SHA256_HEX_SIZE];
  SHA256::sha256_hex("", 0, output);

  const char *expected =
      "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";
  EXPECT_STREQ(output, expected);
}

// Test large input handling
TEST(SHA256_Functions, Sha256Hex_LargeInput) {
  std::vector<uint8_t> large_input(10000, 0xAA);
  char output[SHA256::SHA256_HEX_SIZE];

  SHA256::sha256_hex(large_input.data(), large_input.size(), output);

  // Should produce a valid 64-char hex string
  EXPECT_EQ(output[64], '\0');
  for (int i = 0; i < 64; ++i) {
    char c = output[i];
    EXPECT_TRUE((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'));
  }
}