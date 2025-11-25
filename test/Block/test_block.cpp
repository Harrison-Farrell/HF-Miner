// Google Test includes
#include <gtest/gtest.h>

// project includes
#include <Block/block.h>
#include <SHA256/sha256.h>

static void compute_sha256(const uint8_t *in, size_t len, uint8_t out[32]) {
  SHA256::sha256_bytes(in, len, out);
}

TEST(BlockTEST, convertLittleEndianToBigEndian) {
  uint32_t value = 0x1A2B3C4D;
  uint32_t expected = 0x4D3C2B1A;

  Block::Block block;
  block.swap32(value);

  EXPECT_EQ(value, expected);
}

TEST(BlockTEST, convertBigEndianToLittleEndian) {
  uint32_t value = 0x1A2B3C4D;
  uint32_t expected = 0x4D3C2B1A;

  Block::Block block;
  block.swap32(value);

  EXPECT_EQ(value, expected);
}

TEST(BlockTEST, doubleSHA256Computation) {
  // Input data
  uint8_t data1[32] = {0};
  uint8_t data2[32] = {1};

  // Expected double SHA-256 result
  uint8_t expected_hash[32];
  uint8_t concat[64];
  std::copy(data1, data1 + 32, concat);
  std::copy(data2, data2 + 32, concat + 32);
  uint8_t hash1[32];
  compute_sha256(concat, 64, hash1);
  compute_sha256(hash1, 32, expected_hash);

  // Compute using Block::doubleSHA256
  Block::Block block;
  Block::Hash left, right;
  std::copy(data1, data1 + 32, left.begin());
  std::copy(data2, data2 + 32, right.begin());
  Block::Hash result = block.doubleSHA256(left, right);

  EXPECT_EQ(std::vector<uint8_t>(result.begin(), result.end()),
            std::vector<uint8_t>(expected_hash, expected_hash + 32));
}