// system includes
#include <algorithm>
#include <array>
#include <cstring>
#include <string>
#include <vector>

// Google Test includes
#include <gtest/gtest.h>

// project includes
#include <SHA256/sha256.h>

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
  char output[SHA256_HEX_SIZE];

  for (const auto &tv : NIST_VECTORS) {
    // Dirt-ify buffer to ensure null-terminator is actually written
    memset(output, 'X', SHA256_HEX_SIZE);

    SHA256::sha256_hex(tv.input.data(), tv.input.size(), output);

    EXPECT_STREQ(output, tv.expected_hex.c_str()) << "Failed on: " << tv.name;
    EXPECT_EQ(output[64], '\0') << "Failed to null terminate: " << tv.name;
  }
}

TEST(SHA256_OneShot, Bytes_StandardVectors) {
  uint8_t output[SHA256_BYTES_SIZE];

  for (const auto &tv : NIST_VECTORS) {
    SHA256::sha256_bytes(tv.input.data(), tv.input.size(), output);

    std::string hex_result = bytes_to_hex_string(output, SHA256_BYTES_SIZE);
    EXPECT_EQ(hex_result, tv.expected_hex) << "Failed on: " << tv.name;
  }
}

TEST(SHA256_Streaming, Fragmentation_Consistency) {
  // Ensure hashing "abc" in one go is same as "a" + "b" + "c"
  struct SHA256::sha256 ctx;
  char output[SHA256_HEX_SIZE];

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
  struct SHA256::sha256 ctx;
  char output[SHA256_HEX_SIZE];

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
  struct SHA256::sha256 ctx;
  uint8_t output[SHA256_BYTES_SIZE];

  SHA256::sha256_init(&ctx);
  SHA256::sha256_append(&ctx, "abc", 3);
  SHA256::sha256_finalize_bytes(&ctx, output);

  std::string hex = bytes_to_hex_string(output, SHA256_BYTES_SIZE);
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
  char output[SHA256_HEX_SIZE];

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
  char output[SHA256_HEX_SIZE];

  SHA256::sha256_hex(input.data(), input.size(), output);

  // Verified via sha256sum
  const char *expected =
      "b35439a4ac6f0948b6d6f9e3c6af0f5f590ce20f1bde7090ef7970686ec6738a";
  EXPECT_STREQ(output, expected);
}

TEST(SHA256_EdgeCases, LongInput_MultiBlock) {
  // 200 bytes ensures we process 3 full blocks (64*3=192) + remainder
  std::string input(200, 'A');
  char output[SHA256_HEX_SIZE];

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

  SHA256::Hash expected_result_A = {
      0b10011110, 0b10111010, 0b00111000, 0b10010011, 0b01011000, 0b00100000,
      0b00010001, 0b10101000, 0b10011110, 0b00110011, 0b00110000, 0b10001101,
      0b10100110, 0b00011001, 0b10100101, 0b11111001, 0b01111001, 0b01011000,
      0b00010010, 0b10000101, 0b00100100, 0b11001100, 0b00110011, 0b10101001,
      0b10011011, 0b01101000, 0b10011101, 0b00100100, 0b10101101, 0b01100000,
      0b00000111, 0b00011010};

  SHA256::Hash result_A = SHA256::hashStringToArray(input_hex);
  EXPECT_EQ(result_A, expected_result_A);

  // 00000111 10100011 01011000 11010111 01010010 01001010 00001000 11001011
  // 11000110 10000110 00100100 01010111 10100110 00001010 00011110 11000111
  // 10111000 01110110 00000111 01010001 10011100 01011000 00101001 10011011
  // 11000100 10011001 01011001 11010011 00000000 11101010 10111101 01010010
  input_hex =
      "07a358d7524a08cbc6862457a60a1ec7b87607519c58299bc49959d300eabd52";

  SHA256::Hash expected_result_B = {
      0b00000111, 0b10100011, 0b01011000, 0b11010111, 0b01010010, 0b01001010,
      0b00001000, 0b11001011, 0b11000110, 0b10000110, 0b00100100, 0b01010111,
      0b10100110, 0b00001010, 0b00011110, 0b11000111, 0b10111000, 0b01110110,
      0b00000111, 0b01010001, 0b10011100, 0b01011000, 0b00101001, 0b10011011,
      0b11000100, 0b10011001, 0b01011001, 0b11010011, 0b00000000, 0b11101010,
      0b10111101, 0b01010010};

  SHA256::Hash result_B = SHA256::hashStringToArray(input_hex);
  EXPECT_EQ(result_B, expected_result_B);
}

TEST(SHA256_Conversation, BytesToHex) {
  SHA256::Hash input = {
      0b10011110, 0b10111010, 0b00111000, 0b10010011, 0b01011000, 0b00100000,
      0b00010001, 0b10101000, 0b10011110, 0b00110011, 0b00110000, 0b10001101,
      0b10100110, 0b00011001, 0b10100101, 0b11111001, 0b01111001, 0b01011000,
      0b00010010, 0b10000101, 0b00100100, 0b11001100, 0b00110011, 0b10101001,
      0b10011011, 0b01101000, 0b10011101, 0b00100100, 0b10101101, 0b01100000,
      0b00000111, 0b00011010};

  std::string expected_hex =
      "9eba3893582011a89e33308da619a5f97958128524cc33a99b689d24ad60071a";

  std::string result_hex = SHA256::hashArrayToString(input);
  EXPECT_EQ(result_hex, expected_hex);
}