#ifndef __BLOCK_H__
#define __BLOCK_H__

// system includes
#include <bit>
#include <cstdint>

namespace Block {

class Block {
public:
  /// \brief Default constructor. Initializes block fields to sensible defaults
  /// (zeros).
  Block();

  /// \brief Destructor.
  ~Block();

  // Getters
  /// \brief Get the block version.
  /// \return 32-bit version number.
  uint32_t getVersion() const;

  /// \brief Get pointer to the previous block hash (32 bytes).
  /// \return Pointer to a 32-byte array containing the previous block hash.
  const uint8_t *getPrevBlockHash() const;

  /// \brief Get pointer to the Merkle root (32 bytes).
  /// \return Pointer to a 32-byte array containing the Merkle root.
  const uint8_t *getMerkleRoot() const;

  /// \brief Get the block timestamp.
  /// \return 32-bit UNIX epoch timestamp.
  uint32_t getTimestamp() const;

  /// \brief Get the encoded difficulty target (bits).
  /// \return 32-bit bits field.
  uint32_t getBits() const;

  /// \brief Get the block nonce.
  /// \return 32-bit nonce value.
  uint32_t getNonce() const;

  /// \brief Convert a 32-bit value from big-endian to little-endian in place.
  /// \param bigEndianValue Reference to the 32-bit value (interpreted as
  /// big-endian).
  /// \note The value is replaced with its little-endian representation.
  inline void convertBigEndianToLittleEndian(uint32_t &bigEndianValue) {
    bigEndianValue = std::byteswap(bigEndianValue);
  }

  /// \brief Convert a 32-bit value from little-endian to big-endian in place.
  /// \param littleEndianValue Reference to the 32-bit value (interpreted as
  /// little-endian).
  /// \note The value is replaced with its big-endian representation.
  inline void convertLittleEndianToBigEndian(uint32_t &littleEndianValue) {
    littleEndianValue = std::byteswap(littleEndianValue);
  }

private:
  // Block data members
  uint32_t mVersion;
  uint8_t mPrevBlockHash[32];
  uint8_t mMerkleRoot[32];
  uint32_t mTimestamp;
  uint32_t mBits;
  uint32_t mNonce;
};

} // namespace Block
#endif // __BLOCK_H__