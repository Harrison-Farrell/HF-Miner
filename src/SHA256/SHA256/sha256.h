#ifndef __SHA256_H__
#define __SHA256_H__

// system includes
#include <stdint.h>

#include <array>
#include <string>

namespace SHA256 {
#define SHA256_HEX_SIZE (64 + 1) // 64 hex chars + null terminator
#define SHA256_BYTES_SIZE 32

using Hash = std::array<unsigned char, 32>;

/// \brief Converts a 64-character hexadecimal string into a 32-byte array
/// \param hex_string The 64-character hexadecimal hash string.
/// \return std::array<unsigned char, 32> The byte array representation.
/// \throws std::invalid_argument if not 64 characters long.
Hash hashStringToArray(const std::string &hex_string);

/// \brief Converts a 32-byte std::array into 64-character hexadecimal string.
/// \param bytes The 32-byte array representation of the hash.
/// \return std::string The 64-character hexadecimal hash string.
std::string hashArrayToString(const Hash &bytes);

/// \brief Compute the SHA-256 and write it as a hex string.
/// \param src Pointer to the input data buffer.
/// \param n_bytes Number of bytes to read from src.
/// \param dst_hex65 Destination buffer to receive the hexadecimal digest.
/// Must be at least SHA256_HEX_SIZE bytes.
/// \note The output is 64 hex characters plus a null terminator.
void sha256_hex(const void *src, size_t n_bytes, char *dst_hex65);

/// \brief Compute the SHA-256 write the raw 32-byte digest.
/// \param src Pointer to the input data buffer.
/// \param n_bytes Number of bytes to read from src.
/// \param dst_bytes32 Destination buffer to receive the 32-byte digest. Must be
/// at least SHA256_BYTES_SIZE bytes.
void sha256_bytes(const void *src, size_t n_bytes, void *dst_bytes32);

typedef struct sha256 {
  uint32_t state[8];
  uint8_t buffer[64];
  uint64_t n_bits;
  uint8_t buffer_counter;
} sha256;

/// \brief Initialize a streaming SHA-256 context.
/// \param sha Pointer to a sha256 structure to initialize.
/// \note After initialization the context is ready for sha256_append().
void sha256_init(struct sha256 *sha);

/// \brief Append data to a streaming SHA-256 context.
/// \param sha Pointer to an initialized sha256 context.
/// \param data Pointer to the data to append.
/// \param n_bytes Number of bytes to append.
void sha256_append(struct sha256 *sha, const void *data, size_t n_bytes);

/// \brief Finalize a streaming SHA-256 and write as a hex string.
/// \param sha Pointer to a sha256 context that has been fed with data.
/// \param dst_hex65 Destination buffer to receive the hexadecimal digest. Must
/// be at least SHA256_HEX_SIZE bytes.
/// \note After finalization the context should be reinitialized before reuse.
void sha256_finalize_hex(struct sha256 *sha, char *dst_hex65);

/// \brief Finalize a streaming SHA-256 and write as the raw 32-byte
/// \param sha Pointer to a sha256 context that has been fed with data.
/// \param dst_bytes32 Destination buffer to receive the 32-byte digest. Must be
/// at least SHA256_BYTES_SIZE bytes.
/// \note After finalization the context should be reinitialized before reuse.
void sha256_finalize_bytes(struct sha256 *sha, void *dst_bytes32);

} // namespace SHA256
#endif // __SHA256_H__