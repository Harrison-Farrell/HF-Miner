#ifndef __SHA256_H__
#define __SHA256_H__

// system includes
#include <stdint.h>

#include <array>
#include <string>
#include <vector>

// project includes
#include "types/types.h"

namespace SHA256 {
static constexpr int SHA256_BYTES_SIZE = 32;   // 32 bytes
static constexpr int SHA256_HEX_SIZE = 64 + 1; // 64 hex chars + null terminator

/// \brief SHA256 hash computation class providing static one-shot methods
/// and a streaming context interface.
class SHA256 {
public:
  /// \brief Context structure for streaming SHA-256 computation.
  struct Context {
    uint32_t state[8];
    uint8_t buffer[64];
    uint64_t n_bits;
    uint8_t buffer_counter;
  };

  // Static one-shot methods

  /// \brief Compute SHA-256 hash and return as hexadecimal string.
  /// \param src Pointer to the input data buffer.
  /// \param n_bytes Number of bytes to read from src.
  /// \param dst_hex65 Destination buffer to receive the hexadecimal digest.
  /// Must be at least SHA256_HEX_SIZE bytes.
  /// \note The output is 64 hex characters plus a null terminator.
  static void hex(const void *src, size_t n_bytes, char *dst_hex65);

  /// \brief Compute SHA-256 hash and return as raw bytes.
  /// \param src Pointer to the input data buffer.
  /// \param n_bytes Number of bytes to read from src.
  /// \param dst_bytes32 Destination buffer to receive the 32-byte digest.
  /// Must be at least SHA256_BYTES_SIZE bytes.
  static void bytes(const void *src, size_t n_bytes, void *dst_bytes32);

  // Streaming context methods

  /// \brief Initialize a streaming SHA-256 context.
  /// \param ctx Reference to a Context to initialize.
  /// \note After initialization the context is ready for append().
  static void init(Context &ctx);

  /// \brief Append data to a streaming SHA-256 context.
  /// \param ctx Reference to an initialized Context.
  /// \param data Pointer to the data to append.
  /// \param n_bytes Number of bytes to append.
  static void append(Context &ctx, const void *data, size_t n_bytes);

  /// \brief Finalize a streaming SHA-256 and write as a hex string.
  /// \param ctx Reference to a Context that has been fed with data.
  /// \param dst_hex65 Destination buffer to receive the hexadecimal digest.
  /// Must be at least SHA256_HEX_SIZE bytes.
  /// \note After finalization the context should be reinitialized before reuse.
  static void finalize_hex(Context &ctx, char *dst_hex65);

  /// \brief Finalize a streaming SHA-256 and write as raw bytes.
  /// \param ctx Reference to a Context that has been fed with data.
  /// \param dst_bytes32 Destination buffer to receive the 32-byte digest.
  /// Must be at least SHA256_BYTES_SIZE bytes.
  /// \note After finalization the context should be reinitialized before reuse.
  static void finalize_bytes(Context &ctx, void *dst_bytes32);

  // Conversion utility methods

  /// \brief Converts a 64-character hexadecimal string into a 32-byte array.
  /// \param hex_string The 64-character hexadecimal hash string.
  /// \return std::array<unsigned char, 32> The byte array representation.
  /// \throws std::invalid_argument if not 64 characters long.
  static Hash hashStringToArray(const std::string &hex_string);

  /// \brief Converts a 32-byte std::array into 64-character hexadecimal string.
  /// \param bytes The 32-byte array representation of the hash.
  /// \return std::string The 64-character hexadecimal hash string.
  static std::string hashArrayToString(const Hash &bytes);

private:
  // Private helper methods
  static void sha256_block(Context *ctx);
  static void sha256_append_byte(Context *ctx, uint8_t byte);
  static void sha256_finalize(Context *ctx);
};
// C-style wrapper functions for easier integration
typedef SHA256::Context sha256;

inline void sha256_init(sha256 *sha) { SHA256::init(*sha); }

inline void sha256_append(sha256 *sha, const void *data, size_t n_bytes) {
  SHA256::append(*sha, data, n_bytes);
}

inline void sha256_finalize_hex(sha256 *sha, char *dst_hex65) {
  SHA256::finalize_hex(*sha, dst_hex65);
}

inline void sha256_finalize_bytes(sha256 *sha, void *dst_bytes32) {
  SHA256::finalize_bytes(*sha, dst_bytes32);
}

inline void sha256_hex(const void *src, size_t n_bytes, char *dst_hex65) {
  SHA256::hex(src, n_bytes, dst_hex65);
}

inline void sha256_bytes(const void *src, size_t n_bytes, void *dst_bytes32) {
  SHA256::bytes(src, n_bytes, dst_bytes32);
}

inline Hash hashStringToArray(const std::string &hex_string) {
  return SHA256::hashStringToArray(hex_string);
}

inline std::string hashArrayToString(const Hash &bytes) {
  return SHA256::hashArrayToString(bytes);
}

} // namespace SHA256
#endif // __SHA256_H__