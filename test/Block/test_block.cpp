// Google Test includes
#include <gtest/gtest.h>

// project includes
#include "block/block.h"
#include "sha256/sha256.h"

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

// Test setVersion and getVersion
TEST(BlockTEST, setVersion_getVersion) {
  Block::Block block;

  // Test BLOCK_VERSION_1
  block.setVersion(BLOCK_VERSION_1);
  EXPECT_EQ(block.getVersion(), BLOCK_VERSION_1);

  // Test BLOCK_VERSION_2
  block.setVersion(BLOCK_VERSION_2);
  EXPECT_EQ(block.getVersion(), BLOCK_VERSION_2);

  // Test BLOCK_VERSION_3
  block.setVersion(BLOCK_VERSION_3);
  EXPECT_EQ(block.getVersion(), BLOCK_VERSION_3);

  // Test BLOCK_VERSION_4
  block.setVersion(BLOCK_VERSION_4);
  EXPECT_EQ(block.getVersion(), BLOCK_VERSION_4);

  // Test arbitrary value
  uint32_t arbitrary = 0xDEADBEEF;
  block.setVersion(arbitrary);
  EXPECT_EQ(block.getVersion(), arbitrary);
}

// Test setPrevBlockHash and getPrevBlockHash
TEST(BlockTEST, setPrevBlockHash_getPrevBlockHash) {
  Block::Block block;
  Block::Hash test_hash;

  // Fill with pattern
  for (size_t i = 0; i < test_hash.size(); ++i) {
    test_hash[i] = static_cast<unsigned char>(i);
  }

  block.setPrevBlockHash(test_hash);
  Block::Hash retrieved = block.getPrevBlockHash();

  EXPECT_EQ(retrieved, test_hash);
  for (size_t i = 0; i < 32; ++i) {
    EXPECT_EQ(retrieved[i], static_cast<unsigned char>(i));
  }
}

// Test setMerkleRoot and getMerkleRoot
TEST(BlockTEST, setMerkleRoot_getMerkleRoot) {
  Block::Block block;
  Block::Hash merkle_hash;

  // Fill with pattern
  for (size_t i = 0; i < merkle_hash.size(); ++i) {
    merkle_hash[i] = static_cast<unsigned char>(0xFF - i);
  }

  block.setMerkleRoot(merkle_hash);
  Block::Hash retrieved = block.getMerkleRoot();

  EXPECT_EQ(retrieved, merkle_hash);
  for (size_t i = 0; i < 32; ++i) {
    EXPECT_EQ(retrieved[i], static_cast<unsigned char>(0xFF - i));
  }
}

// Test setTimestamp and getTimestamp
TEST(BlockTEST, setTimestamp_getTimestamp) {
  Block::Block block;

  // Test various timestamps
  uint32_t timestamps[] = {0, 1, 0x7FFFFFFF, 0xFFFFFFFF, 1234567890};

  for (uint32_t ts : timestamps) {
    block.setTimestamp(ts);
    EXPECT_EQ(block.getTimestamp(), ts);
  }
}

// Test setBits and getBits
TEST(BlockTEST, setBits_getBits) {
  Block::Block block;

  // Test various bit values
  uint32_t bits_values[] = {0, 0x00000000, 0x207FFFFF, 0xFFFFFFFF, 0x1d00ffff};

  for (uint32_t bits : bits_values) {
    block.setBits(bits);
    EXPECT_EQ(block.getBits(), bits);
  }
}

// Test setNonce and getNonce
TEST(BlockTEST, setNonce_getNonce) {
  Block::Block block;

  // Test various nonce values
  uint32_t nonces[] = {0, 1, 0x7FFFFFFF, 0xFFFFFFFF, 0xDEADBEEF};

  for (uint32_t nonce : nonces) {
    block.setNonce(nonce);
    EXPECT_EQ(block.getNonce(), nonce);
  }
}

// Test createMerkleRoot with single transaction
TEST(BlockTEST, createMerkleRoot_singleTransaction) {
  Block::Block block;
  std::vector<Block::Hash> tx_hashes;

  Block::Hash tx1;
  tx1.fill(0x01);
  tx_hashes.push_back(tx1);

  Block::Hash merkle = block.createMerkleRoot(tx_hashes);

  // For single transaction, merkle root equals the transaction itself
  EXPECT_EQ(merkle, tx1);
}

// Test createMerkleRoot with two transactions
TEST(BlockTEST, createMerkleRoot_twoTransactions) {
  Block::Block block;
  std::vector<Block::Hash> tx_hashes;

  Block::Hash tx1, tx2;
  tx1.fill(0x01);
  tx2.fill(0x02);
  tx_hashes.push_back(tx1);
  tx_hashes.push_back(tx2);

  Block::Hash merkle = block.createMerkleRoot(tx_hashes);

  // Merkle root should be double SHA256 of the two transactions
  Block::Hash expected = block.doubleSHA256(tx1, tx2);
  EXPECT_EQ(merkle, expected);
}

// Test createMerkleRoot with three transactions (odd number)
TEST(BlockTEST, createMerkleRoot_threeTransactions) {
  Block::Block block;
  std::vector<Block::Hash> tx_hashes;

  Block::Hash tx1, tx2, tx3;
  tx1.fill(0x01);
  tx2.fill(0x02);
  tx3.fill(0x03);
  tx_hashes.push_back(tx1);
  tx_hashes.push_back(tx2);
  tx_hashes.push_back(tx3);

  Block::Hash merkle = block.createMerkleRoot(tx_hashes);

  // First level: combine tx1-tx2, and duplicate tx3
  Block::Hash level1_left = block.doubleSHA256(tx1, tx2);
  Block::Hash level1_right = block.doubleSHA256(tx3, tx3);

  // Root: combine level1_left and level1_right
  Block::Hash expected = block.doubleSHA256(level1_left, level1_right);
  EXPECT_EQ(merkle, expected);
}

// Test createMerkleRoot with four transactions
TEST(BlockTEST, createMerkleRoot_fourTransactions) {
  Block::Block block;
  std::vector<Block::Hash> tx_hashes;

  Block::Hash tx1, tx2, tx3, tx4;
  tx1.fill(0x01);
  tx2.fill(0x02);
  tx3.fill(0x03);
  tx4.fill(0x04);
  tx_hashes.push_back(tx1);
  tx_hashes.push_back(tx2);
  tx_hashes.push_back(tx3);
  tx_hashes.push_back(tx4);

  Block::Hash merkle = block.createMerkleRoot(tx_hashes);

  // First level: combine pairs
  Block::Hash level1_left = block.doubleSHA256(tx1, tx2);
  Block::Hash level1_right = block.doubleSHA256(tx3, tx4);

  // Root: combine level1
  Block::Hash expected = block.doubleSHA256(level1_left, level1_right);
  EXPECT_EQ(merkle, expected);
}

// Test createMerkleRoot with empty transactions
TEST(BlockTEST, createMerkleRoot_emptyTransactions) {
  Block::Block block;
  std::vector<Block::Hash> tx_hashes;

  Block::Hash merkle = block.createMerkleRoot(tx_hashes);

  // Should return zeroed hash
  Block::Hash expected;
  expected.fill(0x00);
  EXPECT_EQ(merkle, expected);
}

// Test constructor initializes to defaults
TEST(BlockTEST, Constructor_InitializesDefaults) {
  Block::Block block;

  EXPECT_EQ(block.getVersion(), 0);
  EXPECT_EQ(block.getTimestamp(), 0);
  EXPECT_EQ(block.getBits(), 0);
  EXPECT_EQ(block.getNonce(), 0);

  // Check prev block hash is zeroed
  Block::Hash expected_zero;
  expected_zero.fill(0x00);
  EXPECT_EQ(block.getPrevBlockHash(), expected_zero);
  EXPECT_EQ(block.getMerkleRoot(), expected_zero);
}

// Test all setters and getters together (integration)
TEST(BlockTEST, AllSettersGetters_Integration) {
  Block::Block block;

  // Set all values
  uint32_t version = BLOCK_VERSION_4;
  uint32_t timestamp = 1234567890;
  uint32_t bits = 0x1d00ffff;
  uint32_t nonce = 0xDEADBEEF;

  Block::Hash prev_hash, merkle_hash;
  for (size_t i = 0; i < 32; ++i) {
    prev_hash[i] = static_cast<unsigned char>(i);
    merkle_hash[i] = static_cast<unsigned char>(255 - i);
  }

  block.setVersion(version);
  block.setTimestamp(timestamp);
  block.setBits(bits);
  block.setNonce(nonce);
  block.setPrevBlockHash(prev_hash);
  block.setMerkleRoot(merkle_hash);

  // Verify all values
  EXPECT_EQ(block.getVersion(), version);
  EXPECT_EQ(block.getTimestamp(), timestamp);
  EXPECT_EQ(block.getBits(), bits);
  EXPECT_EQ(block.getNonce(), nonce);
  EXPECT_EQ(block.getPrevBlockHash(), prev_hash);
  EXPECT_EQ(block.getMerkleRoot(), merkle_hash);
}