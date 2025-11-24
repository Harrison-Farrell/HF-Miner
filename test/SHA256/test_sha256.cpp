// system includes
#include <algorithm>
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
    // Dirtify buffer to ensure null-terminator is actually written
    memset(output, 'X', SHA256_HEX_SIZE);

    sha256_hex(tv.input.data(), tv.input.size(), output);

    EXPECT_STREQ(output, tv.expected_hex.c_str()) << "Failed on: " << tv.name;
    EXPECT_EQ(output[64], '\0') << "Failed to null terminate: " << tv.name;
  }
}

TEST(SHA256_OneShot, Bytes_StandardVectors) {
  uint8_t output[SHA256_BYTES_SIZE];

  for (const auto &tv : NIST_VECTORS) {
    sha256_bytes(tv.input.data(), tv.input.size(), output);

    std::string hex_result = bytes_to_hex_string(output, SHA256_BYTES_SIZE);
    EXPECT_EQ(hex_result, tv.expected_hex) << "Failed on: " << tv.name;
  }
}

TEST(SHA256_Streaming, Fragmentation_Consistency) {
  // Ensure hashing "abc" in one go is same as "a" + "b" + "c"
  struct sha256 ctx;
  char output[SHA256_HEX_SIZE];

  sha256_init(&ctx);
  sha256_append(&ctx, "a", 1);
  sha256_append(&ctx, "b", 1);
  sha256_append(&ctx, "c", 1);
  sha256_finalize_hex(&ctx, output);

  EXPECT_STREQ(
      output,
      "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
}

TEST(SHA256_Streaming, ContextReuse) {
  // Ensure calling init() on a dirty struct resets it correctly
  struct sha256 ctx;
  char output[SHA256_HEX_SIZE];

  // First usage
  sha256_init(&ctx);
  sha256_append(&ctx, "abc", 3);
  sha256_finalize_hex(&ctx, output);

  // Reuse
  sha256_init(&ctx);
  sha256_append(&ctx, "", 0); // Empty string
  sha256_finalize_hex(&ctx, output);

  // Expect empty string hash
  EXPECT_STREQ(
      output,
      "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
}

TEST(SHA256_Streaming, FinalizeBytes_Works) {
  struct sha256 ctx;
  uint8_t output[SHA256_BYTES_SIZE];

  sha256_init(&ctx);
  sha256_append(&ctx, "abc", 3);
  sha256_finalize_bytes(&ctx, output);

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

  sha256_hex(input.data(), input.size(), output);

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

  sha256_hex(input.data(), input.size(), output);

  // Verified via sha256sum
  const char *expected =
      "b35439a4ac6f0948b6d6f9e3c6af0f5f590ce20f1bde7090ef7970686ec6738a";
  EXPECT_STREQ(output, expected);
}

TEST(SHA256_EdgeCases, LongInput_MultiBlock) {
  // 200 bytes ensures we process 3 full blocks (64*3=192) + remainder
  std::string input(200, 'A');
  char output[SHA256_HEX_SIZE];

  sha256_hex(input.data(), input.size(), output);

  // Verified via sha256sum
  const char *expected =
      "70d3bf8b0b9d83a61012f35fbf460c4207063fe31b4d6178390fe3b721cc03f7";
  EXPECT_STREQ(output, expected);
}