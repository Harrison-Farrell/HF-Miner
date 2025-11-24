#include <Block/block.h>

// system includes
#include <algorithm>

Block::Block::Block() : mVersion(0), mTimestamp(0), mBits(0), mNonce(0) {
  // Initialize previous block hash and Merkle root to zeros
  std::fill(std::begin(mPrevBlockHash), std::end(mPrevBlockHash), 0);
  std::fill(std::begin(mMerkleRoot), std::end(mMerkleRoot), 0);
}

Block::Block::~Block() {}

uint32_t Block::Block::getVersion() const { return mVersion; }

const uint8_t *Block::Block::getPrevBlockHash() const { return mPrevBlockHash; }

const uint8_t *Block::Block::getMerkleRoot() const { return mMerkleRoot; }

uint32_t Block::Block::getTimestamp() const { return mTimestamp; }

uint32_t Block::Block::getBits() const { return mBits; }

uint32_t Block::Block::getNonce() const { return mNonce; }