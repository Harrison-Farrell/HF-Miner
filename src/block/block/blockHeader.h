#ifndef __BLOCK_H__
#define __BLOCK_H__

// system includes
#include <array>
#include <bit>
#include <cstdint>
#include <vector>

// project includes
#include "types/types.h"

namespace Block {

#define BLOCK_VERSION_1 0x00000001
#define BLOCK_VERSION_2 0x00000002  // reference: BIP34
#define BLOCK_VERSION_3 0x00000003  // reference: BIP66
#define BLOCK_VERSION_4 0x00000004  // reference: BIP65

class BlockHeader {
 public:
  /// \brief Default constructor. Initializes block fields to sensible defaults
  /// (zeros).
  BlockHeader();

  /// \brief Destructor.
  ~BlockHeader();

  // Setters
  /// \brief Set the block version.
  /// \param version 32-bit version number to store in the block.
  inline void setVersion(uint32_t version) { mVersion = version; }

  /// \brief Set the previous block hash.
  /// \param prev_block_hash 32-byte array containing the previous block hash.
  /// \note The data is copied into the block's internal buffer.
  inline void setPrevBlockHash(const Hash &prev_block_hash) {
    mPrevBlockHash = prev_block_hash;
  }

  /// \brief Set the Merkle root.
  /// \param merkle_root 32-byte array containing the Merkle root.
  /// \note The data is copied into the block's internal buffer.
  inline void setMerkleRoot(const Hash &merkle_root) {
    mMerkleRoot = merkle_root;
  }

  /// \brief Compute the Merkle root from a list of transaction hashes.
  /// \param tx_hashes Vector of 32-byte transaction hashes.
  /// \return 32-byte buffer containing the computed Merkle root.
  Hash createMerkleRoot(const std::vector<Hash> &tx_hashes);

  /// \brief Set the block timestamp.
  /// \param timestamp 32-bit UNIX epoch timestamp to store in the block.
  inline void setTimestamp(uint32_t timestamp) { mTimestamp = timestamp; }

  /// \brief Set the encoded difficulty target (bits).
  /// \param bits 32-bit bits field to store in the block.
  inline void setBits(uint32_t bits) { mBits = bits; }

  /// \brief Set the block nonce.
  /// \param nonce 32-bit nonce value to store in the block.
  inline void setNonce(uint32_t nonce) { mNonce = nonce; }

  // Getters
  /// \brief Get the block version.
  /// \return 32-bit version number.
  inline uint32_t getVersion() const { return mVersion; }

  /// \brief Get pointer to the previous block hash (32 bytes).
  /// \return Pointer to a 32-byte array containing the previous block hash.
  inline const Hash getPrevBlockHash() const { return mPrevBlockHash; }

  /// \brief Get pointer to the Merkle root (32 bytes).
  /// \return Pointer to a 32-byte array containing the Merkle root.
  inline const Hash getMerkleRoot() const { return mMerkleRoot; }

  /// \brief Get the block timestamp.
  /// \return 32-bit UNIX epoch timestamp.
  inline uint32_t getTimestamp() const { return mTimestamp; }

  /// \brief Get the encoded difficulty target (bits).
  /// \return 32-bit bits field.
  inline uint32_t getBits() const { return mBits; }

  /// \brief Get the block nonce.
  /// \return 32-bit nonce value.
  inline uint32_t getNonce() const { return mNonce; }

  /// \brief Convert a 32-bit value either little-endian to big-endian in place.
  /// \param value Reference to the 32-bit value
  /// \note The value is replaced with its reverse representation.
  static inline void swap32(uint32_t &value) { value = std::byteswap(value); }

  // Helper function to compute double SHA-256 of two concatenated hashes
  Hash doubleSHA256(const Hash &left, const Hash &right);
  std::vector<Hash> recursiveMerkleCompute(const std::vector<Hash> &hashes);

  /// \brief Calculate a valid nonce for the block header using proof-of-work.
  /// \param maxAttempts Maximum number of nonce attempts before giving up.
  /// \return true if a valid nonce was found, false if maxAttempts was
  /// exceeded.
  /// \note Modifies mNonce to the calculated valid value on success.
  bool calculateNonce(uint32_t maxAttempts = 0xFFFFFFFF);

  /// \brief Calculate the hash of the block header.
  /// \return The computed double SHA-256 hash of the block header.
  Hash calculateBlockHash() const;

 private:
  // Block data members
  uint32_t mVersion;    // little-endian
  Hash mPrevBlockHash;  // natural byte order
  Hash mMerkleRoot;     // natural byte order
  uint32_t mTimestamp;  // little-endian
  uint32_t mBits;       // little-endian
  uint32_t mNonce;      // little-endian

  // Byte sizes of fields
  static constexpr int mBits_bytesize = 4;
  static constexpr int mNonce_bytesize = 4;
  static constexpr int mVersion_bytesize = 4;
  static constexpr int mTimestamp_bytesize = 4;
  static constexpr int mMerkleRoot_bytesize = 32;
  static constexpr int mPrevBlockHash_bytesize = 32;
};

}  // namespace Block
#endif  // __BLOCK_H__