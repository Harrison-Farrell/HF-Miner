// Google Test includes
#include <gtest/gtest.h>

// project includes
#include "block/blockHeader.h"
#include "sha256/sha256.h"
#include "types/types.h"

static void compute_sha256(const uint8_t *in, size_t len, uint8_t out[32]) {
  SHA256::sha256_bytes(in, len, out);
}

TEST(BlockHeaderTEST, convertLittleEndianToBigEndian) {
  uint32_t value = 0x1A2B3C4D;
  uint32_t expected = 0x4D3C2B1A;

  Block::BlockHeader block;
  block.swap32(value);

  EXPECT_EQ(value, expected);
}

TEST(BlockHeaderTEST, convertBigEndianToLittleEndian) {
  uint32_t value = 0x1A2B3C4D;
  uint32_t expected = 0x4D3C2B1A;

  Block::BlockHeader block;
  block.swap32(value);

  EXPECT_EQ(value, expected);
}

TEST(BlockHeaderTEST, doubleSHA256Computation) {
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
  Block::BlockHeader block;
  Hash left, right;
  std::copy(data1, data1 + 32, left.begin());
  std::copy(data2, data2 + 32, right.begin());
  Hash result = block.doubleSHA256(left, right);

  EXPECT_EQ(std::vector<uint8_t>(result.begin(), result.end()),
            std::vector<uint8_t>(expected_hash, expected_hash + 32));
}

// Test setVersion and getVersion
TEST(BlockHeaderTEST, setVersion_getVersion) {
  Block::BlockHeader block;

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
TEST(BlockHeaderTEST, setPrevBlockHash_getPrevBlockHash) {
  Block::BlockHeader block;
  Hash test_hash;

  // Fill with pattern
  for (size_t i = 0; i < test_hash.size(); ++i) {
    test_hash[i] = static_cast<unsigned char>(i);
  }

  block.setPrevBlockHash(test_hash);
  Hash retrieved = block.getPrevBlockHash();

  EXPECT_EQ(retrieved, test_hash);
  for (size_t i = 0; i < 32; ++i) {
    EXPECT_EQ(retrieved[i], static_cast<unsigned char>(i));
  }
}

// Test setMerkleRoot and getMerkleRoot
TEST(BlockHeaderTEST, setMerkleRoot_getMerkleRoot) {
  Block::BlockHeader block;
  Hash merkle_hash;

  // Fill with pattern
  for (size_t i = 0; i < merkle_hash.size(); ++i) {
    merkle_hash[i] = static_cast<unsigned char>(0xFF - i);
  }

  block.setMerkleRoot(merkle_hash);
  Hash retrieved = block.getMerkleRoot();

  EXPECT_EQ(retrieved, merkle_hash);
  for (size_t i = 0; i < 32; ++i) {
    EXPECT_EQ(retrieved[i], static_cast<unsigned char>(0xFF - i));
  }
}

// Test setTimestamp and getTimestamp
TEST(BlockHeaderTEST, setTimestamp_getTimestamp) {
  Block::BlockHeader block;

  // Test various timestamps
  uint32_t timestamps[] = {0, 1, 0x7FFFFFFF, 0xFFFFFFFF, 1234567890};

  for (uint32_t ts : timestamps) {
    block.setTimestamp(ts);
    EXPECT_EQ(block.getTimestamp(), ts);
  }
}

// Test setBits and getBits
TEST(BlockHeaderTEST, setBits_getBits) {
  Block::BlockHeader block;

  // Test various bit values
  uint32_t bits_values[] = {0, 0x00000000, 0x207FFFFF, 0xFFFFFFFF, 0x1d00ffff};

  for (uint32_t bits : bits_values) {
    block.setBits(bits);
    EXPECT_EQ(block.getBits(), bits);
  }
}

// Test setNonce and getNonce
TEST(BlockHeaderTEST, setNonce_getNonce) {
  Block::BlockHeader block;

  // Test various nonce values
  uint32_t nonces[] = {0, 1, 0x7FFFFFFF, 0xFFFFFFFF, 0xDEADBEEF};

  for (uint32_t nonce : nonces) {
    block.setNonce(nonce);
    EXPECT_EQ(block.getNonce(), nonce);
  }
}

// Test createMerkleRoot with single transaction
TEST(BlockHeaderTEST, createMerkleRoot_singleTransaction) {
  Block::BlockHeader block;
  std::vector<Hash> tx_hashes;

  Hash tx1;
  tx1.fill(0x01);
  tx_hashes.push_back(tx1);

  Hash merkle = block.createMerkleRoot(tx_hashes);

  // For single transaction, merkle root equals the transaction itself
  EXPECT_EQ(merkle, tx1);
}

// Test createMerkleRoot with two transactions
TEST(BlockHeaderTEST, createMerkleRoot_twoTransactions) {
  Block::BlockHeader block;
  std::vector<Hash> tx_hashes;

  Hash tx1, tx2;
  tx1.fill(0x01);
  tx2.fill(0x02);
  tx_hashes.push_back(tx1);
  tx_hashes.push_back(tx2);

  Hash merkle = block.createMerkleRoot(tx_hashes);

  // Merkle root should be double SHA256 of the two transactions
  Hash expected = block.doubleSHA256(tx1, tx2);
  EXPECT_EQ(merkle, expected);
}

// Test createMerkleRoot with three transactions (odd number)
TEST(BlockHeaderTEST, createMerkleRoot_threeTransactions) {
  Block::BlockHeader block;
  std::vector<Hash> tx_hashes;

  Hash tx1, tx2, tx3;
  tx1.fill(0x01);
  tx2.fill(0x02);
  tx3.fill(0x03);
  tx_hashes.push_back(tx1);
  tx_hashes.push_back(tx2);
  tx_hashes.push_back(tx3);

  Hash merkle = block.createMerkleRoot(tx_hashes);

  // First level: combine tx1-tx2, and duplicate tx3
  Hash level1_left = block.doubleSHA256(tx1, tx2);
  Hash level1_right = block.doubleSHA256(tx3, tx3);

  // Root: combine level1_left and level1_right
  Hash expected = block.doubleSHA256(level1_left, level1_right);
  EXPECT_EQ(merkle, expected);
}

// Test createMerkleRoot with four transactions
TEST(BlockHeaderTEST, createMerkleRoot_fourTransactions) {
  Block::BlockHeader block;
  std::vector<Hash> tx_hashes;

  Hash tx1, tx2, tx3, tx4;
  tx1.fill(0x01);
  tx2.fill(0x02);
  tx3.fill(0x03);
  tx4.fill(0x04);
  tx_hashes.push_back(tx1);
  tx_hashes.push_back(tx2);
  tx_hashes.push_back(tx3);
  tx_hashes.push_back(tx4);

  Hash merkle = block.createMerkleRoot(tx_hashes);

  // First level: combine pairs
  Hash level1_left = block.doubleSHA256(tx1, tx2);
  Hash level1_right = block.doubleSHA256(tx3, tx4);

  // Root: combine level1
  Hash expected = block.doubleSHA256(level1_left, level1_right);
  EXPECT_EQ(merkle, expected);
}

// Test createMerkleRoot with empty transactions
TEST(BlockHeaderTEST, createMerkleRoot_emptyTransactions) {
  Block::BlockHeader block;
  std::vector<Hash> tx_hashes;

  Hash merkle = block.createMerkleRoot(tx_hashes);

  // Should return zeroed hash
  Hash expected;
  expected.fill(0x00);
  EXPECT_EQ(merkle, expected);
}

// Test constructor initializes to defaults
TEST(BlockHeaderTEST, Constructor_InitializesDefaults) {
  Block::BlockHeader block;

  EXPECT_EQ(block.getVersion(), 0);
  EXPECT_EQ(block.getTimestamp(), 0);
  EXPECT_EQ(block.getBits(), 0);
  EXPECT_EQ(block.getNonce(), 0);

  // Check prev block hash is zeroed
  Hash expected_zero;
  expected_zero.fill(0x00);
  EXPECT_EQ(block.getPrevBlockHash(), expected_zero);
  EXPECT_EQ(block.getMerkleRoot(), expected_zero);
}

// Test all setters and getters together (integration)
TEST(BlockHeaderTEST, AllSettersGetters_Integration) {
  Block::BlockHeader block;

  // Set all values
  uint32_t version = BLOCK_VERSION_4;
  uint32_t timestamp = 1234567890;
  uint32_t bits = 0x1d00ffff;
  uint32_t nonce = 0xDEADBEEF;

  Hash prev_hash, merkle_hash;
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
