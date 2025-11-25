#ifndef __SHA256_H__
#define __SHA256_H__

// system includes
#include <stdint.h>

namespace SHA256 {
#define SHA256_HEX_SIZE (64 + 1)  // 64 hex chars + null terminator
#define SHA256_BYTES_SIZE 32

/// \brief Compute the SHA-256 and write it as a hex string.
/// \param src Pointer to the input data buffer.
/// \param n_bytes Number of bytes to read from src.
/// \param dst_hex65 Destination buffer to receive the hexadecimal digest. Must
/// be at least SHA256_HEX_SIZE bytes.
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

}  // namespace SHA256
#endif  // __SHA256_H__

/*
 /// \brief Converts a 64-character hexadecimal string into a 32-byte array
 /// \param hex_string The 64-character hexadecimal hash string.
 /// \return std::array<unsigned char, 32> The byte array representation.
 /// \throws std::invalid_argument if not 64 characters long.


std::array<unsigned char, 32> hex_string_to_bytes(const std::string& hex_string)
{
    // A full SHA-256 hex string is 64 characters long (32 bytes * 2 hex
chars/byte). if (hex_string.length() != 64) { throw std::invalid_argument("Input
string must be 64 characters long for SHA-256.");
    }

    std::array<unsigned char, 32> bytes;

    // Process two characters at a time
    for (size_t i = 0; i < 32; ++i) {
        // Extract the 2-character hexadecimal substring
        std::string byte_string = hex_string.substr(i * 2, 2);

        // Convert the 2-character hex string to an unsigned long, base 16
        unsigned long value = std::stoul(byte_string, nullptr, 16);

        // Cast the value to an unsigned char and store it
        bytes[i] = static_cast<unsigned char>(value);
    }

    return bytes;
}

/*
 /// \brief Converts a 32-byte std::array into 64-character hexadecimal string.
 /// \param bytes The 32-byte array representation of the hash.
 /// \return std::string The 64-character hexadecimal hash string.


 std::string bytes_to_hex_string(const std::array<unsigned char, 32>& bytes) {
    std::stringstream ss;
    // Set formatting for hexadecimal output
    ss << std::hex << std::setfill('0');

    // Iterate through the array
    for (unsigned char byte : bytes) {
        // Print each byte as a 2-character hex number
        // (e.g., 0x0A becomes "0a", 0xFF becomes "ff")
        ss << std::setw(2) << static_cast<int>(byte);
    }

    return ss.str();
} */