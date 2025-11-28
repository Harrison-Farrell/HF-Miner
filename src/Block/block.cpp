#include "Block/block.h"

// system includes
#include <algorithm>
#include <vector>

// project includes
#include <SHA256/sha256.h>

Block::Block::Block() : mVersion(0), mTimestamp(0), mBits(0), mNonce(0) {
  // Initialize previous block hash and Merkle root to zeros
  std::fill(std::begin(mPrevBlockHash), std::end(mPrevBlockHash), 0);
  std::fill(std::begin(mMerkleRoot), std::end(mMerkleRoot), 0);
}

Block::Block::~Block() {}

Block::Hash Block::Block::doubleSHA256(const Hash &left, const Hash &right) {
  uint8_t concat[64];
  std::copy(left.begin(), left.end(), concat);
  std::copy(right.begin(), right.end(), concat + 32);

  // first SHA-256 pass
  Hash hash1;
  SHA256::sha256_bytes(concat, SHA256_BYTES_SIZE * 2, hash1.data());

  // second SHA-256 pass
  Hash hash2;
  SHA256::sha256_bytes(hash1.data(), SHA256_BYTES_SIZE, hash2.data());

  return hash2;
}

Block::Hash Block::Block::createMerkleRoot(const std::vector<Hash> &tx_hashes) {
  if (tx_hashes.empty()) {
    std::fill(std::begin(mMerkleRoot), std::end(mMerkleRoot), 0);
    return mMerkleRoot;
  }
  mMerkleRoot = recursiveMerkleCompute(tx_hashes).front();
  return mMerkleRoot;
}

std::vector<Block::Hash>
Block::Block::recursiveMerkleCompute(const std::vector<Hash> &hashes) {
  if (hashes.size() == 1) {
    return hashes;
  }

  std::vector<Hash> new_level;
  size_t len = hashes.size();
  new_level.reserve((len + 1) / 2);

  for (size_t i = 0; i < len - 1; i += 2) {
    new_level.push_back(doubleSHA256(hashes[i], hashes[i + 1]));
  }
  if (len % 2 == 1) {
    new_level.push_back(doubleSHA256(hashes[len - 1], hashes[len - 1]));
  }

  return recursiveMerkleCompute(new_level);
}
