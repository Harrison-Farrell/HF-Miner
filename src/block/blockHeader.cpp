#include "block/blockHeader.h"

// system includes
#include <algorithm>
#include <vector>
#include <iostream>

// project includes
#include "sha256/sha256.h"

Block::BlockHeader::BlockHeader()
    : mVersion(0), mTimestamp(0), mBits(0), mNonce(0) {
  // Initialize previous block hash and Merkle root to zeros
  std::fill(std::begin(mPrevBlockHash), std::end(mPrevBlockHash), 0);
  std::fill(std::begin(mMerkleRoot), std::end(mMerkleRoot), 0);
}

Block::BlockHeader::~BlockHeader() {}

Hash Block::BlockHeader::doubleSHA256(const Hash &left, const Hash &right) {
  uint8_t concat[64];
  std::copy(left.begin(), left.end(), concat);
  std::copy(right.begin(), right.end(), concat + 32);

  // first SHA-256 pass
  Hash hash1;
  SHA256::sha256_bytes(concat, SHA256::SHA256_BYTES_SIZE * 2, hash1.data());

  // second SHA-256 pass
  Hash hash2;
  SHA256::sha256_bytes(hash1.data(), SHA256::SHA256_BYTES_SIZE, hash2.data());

  return hash2;
}

Hash Block::BlockHeader::createMerkleRoot(const std::vector<Hash> &tx_hashes) {
  if (tx_hashes.empty()) {
    std::fill(std::begin(mMerkleRoot), std::end(mMerkleRoot), 0);
    return mMerkleRoot;
  }
  mMerkleRoot = recursiveMerkleCompute(tx_hashes).front();
  return mMerkleRoot;
}

std::vector<Hash> Block::BlockHeader::recursiveMerkleCompute(
    const std::vector<Hash> &hashes) {
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

Hash Block::BlockHeader::calculateBlockHash() const {
  // Serialize the block header (80 bytes total)
  uint8_t header[80];
  uint8_t *cursor = header;

  // Version (4 bytes, little-endian)
  uint32_t version = mVersion;
  std::copy(reinterpret_cast<uint8_t *>(&version),
            reinterpret_cast<uint8_t *>(&version) + 4, cursor);
  cursor += mVersion_bytesize;

  // Previous block hash (32 bytes)
  std::copy(mPrevBlockHash.begin(), mPrevBlockHash.end(), cursor);
  cursor += mPrevBlockHash_bytesize;

  // Merkle root (32 bytes)
  std::copy(mMerkleRoot.begin(), mMerkleRoot.end(), cursor);
  cursor += mMerkleRoot_bytesize;

  // Timestamp (4 bytes, little-endian)
  uint32_t timestamp = mTimestamp;
  std::copy(reinterpret_cast<uint8_t *>(&timestamp),
            reinterpret_cast<uint8_t *>(&timestamp) + 4, cursor);
  cursor += mTimestamp_bytesize;

  // Bits (4 bytes, little-endian)
  uint32_t bits = mBits;
  std::copy(reinterpret_cast<uint8_t *>(&bits),
            reinterpret_cast<uint8_t *>(&bits) + 4, cursor);
  cursor += mBits_bytesize;

  // Nonce (4 bytes, little-endian)
  uint32_t nonce = mNonce;
  std::copy(reinterpret_cast<uint8_t *>(&nonce),
            reinterpret_cast<uint8_t *>(&nonce) + mNonce_bytesize, cursor);

  // Compute double SHA-256 of the entire header
  Hash hash;
  SHA256::sha256_bytes(header, 80, hash.data());
  uint8_t hash1[32];
  std::copy(hash.begin(), hash.end(), hash1);
  SHA256::sha256_bytes(hash1, 32, hash.data());

  return hash;
}

bool Block::BlockHeader::findNonce(uint32_t maxAttempts) {
  // Parse the difficulty target from bits
  // bits format: high byte is exponent, next 3 bytes are mantissa
  uint32_t bits = mBits;
  uint32_t exponent = (bits >> 24) & 0xFF;
  uint32_t mantissa = bits & 0x00FFFFFF;

  // Calculate target as mantissa * 2^(8*(exponent-3))
  // For now, we'll use a simple comparison: hash must be less than target
  // Create target hash from bits
  Hash target;
  target.fill(0xFF);

  if (exponent <= 3) [[unlikely]] {
    // Target is less than 1 (should not happen in practice)
    return false;
  }

  // Set the target bytes based on bits encoding
  for (size_t i = 0; i < target.size(); ++i) {
    if (i < (exponent - 3)) {
      target[i] = 0x00;
    } else if (i == (exponent - 3)) {
      target[i] = static_cast<uint8_t>(mantissa & 0xFF);
    } else if (i == (exponent - 2)) {
      target[i] = static_cast<uint8_t>((mantissa >> 8) & 0xFF);
    } else if (i == (exponent - 1)) {
      target[i] = static_cast<uint8_t>((mantissa >> 16) & 0xFF);
    } else {
      target[i] = 0xFF;
    }
  }

  // Try nonces from 0 to maxAttempts
  for (uint32_t attempt = 0; attempt < maxAttempts; ++attempt) {
    setNonce(attempt);
    Hash blockHash = calculateBlockHash();

    // Check if hash is less than target (difficulty met)
    bool valid = true;
    for (int i = static_cast<int>(blockHash.size()) - 1; i >= 0; --i) {
      if (blockHash[i] < target[i]) {
        std::cout << "Valid nonce found: " << attempt << std::endl;
        return true;  // Found valid nonce
      } else if (blockHash[i] > target[i]) {
        break;  // Hash too high, continue to next nonce
      }
      // If equal, continue checking lower bytes
    }
  }

  return false;  // No valid nonce found within maxAttempts
}
