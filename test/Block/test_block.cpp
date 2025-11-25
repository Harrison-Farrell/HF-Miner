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

TEST(BlockTEST, createMerkleRootEvenCount) {
  Block::Hash =
      0x925bcca21a95a015b5a371128610dd3f47cd68be76e2cb023790b2603c9c825e;
  // c7b6fc699a18467d18d3d62b75ce3df6d11334453cc1a047b37a6c18fb725022
  // 6094c3007890c1103bc3863a3928318d15c4e4fdd99f35f0de1d762b010e69e1
  // a5948fe4155a63ee94bb5ad65d4c45b3e14c7fc61acd8cbd524908eaa1391525
  std::vector<Block::Hash> tx_hashes { {}, {1} }
};
Block::Hash tx2 = {1};

std::vector<Block::Hash> tx_hashes = {tx1, tx2};

// Expected Merkle root computation
Block::Block block;
Block::Hash expected_root = block.doubleSHA256(tx1, tx2);

// Compute Merkle root using createMerkleRoot
Block::Hash computed_root = block.createMerkleRoot(tx_hashes);

EXPECT_EQ(std::vector<uint8_t>(computed_root.begin(), computed_root.end()),
          std::vector<uint8_t>(expected_root.begin(), expected_root.end()));
}