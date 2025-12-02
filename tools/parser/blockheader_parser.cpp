// system includes
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

/// \brief Convert hex string to bits (vector of bools)
/// \param hex_string The hexadecimal string to convert
/// \return Vector of bools representing bits
std::vector<bool> hexStringToBits(const std::string& hex_string) {
  std::vector<bool> bits;

  if (hex_string.length() % 2 != 0) {
    throw std::invalid_argument(
        "Hex string must have even number of characters");
  }

  for (size_t i = 0; i < hex_string.length(); i += 2) {
    std::string byte_string = hex_string.substr(i, 2);
    uint8_t byte = static_cast<uint8_t>(std::stoi(byte_string, nullptr, 16));
    // Convert each byte to 8 bits (MSB first)
    for (int j = 7; j >= 0; --j) {
      bits.push_back((byte >> j) & 1);
    }
  }

  return bits;
}

/// \brief Convert bits to hex string
/// \param bits The bits to convert
/// \return Hexadecimal string representation
std::string bitsToHexString(const std::vector<bool>& bits) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0');

  for (size_t i = 0; i < bits.size(); i += 8) {
    uint8_t byte = 0;
    for (int j = 0; j < 8 && i + j < bits.size(); ++j) {
      byte = (byte << 1) | (bits[i + j] ? 1 : 0);
    }
    ss << std::setw(2) << static_cast<int>(byte);
  }

  return ss.str();
}

/// \brief Print bits in big-endian format (reversed by byte) as hex
/// \param bits The bits to print (from bit offset to bit offset + num_bits)
/// \param bit_offset Starting bit index
/// \param num_bits Number of bits to print
/// \param name The name of the field
/// \param logger The spdlog logger instance
void printBitsReversed(const std::vector<bool>& bits, size_t bit_offset,
                       size_t num_bits, const std::string& name,
                       std::shared_ptr<spdlog::logger>& logger) {
  std::stringstream ss;
  ss << name << ": ";
  ss << std::hex << std::setfill('0');

  // Print bits in reverse byte order (big-endian display) as hex
  for (int byte_idx = (num_bits / 8) - 1; byte_idx >= 0; --byte_idx) {
    uint8_t byte = 0;
    for (int bit_in_byte = 0; bit_in_byte < 8; ++bit_in_byte) {
      size_t bit_idx = bit_offset + byte_idx * 8 + bit_in_byte;
      byte = (byte << 1) | (bits[bit_idx] ? 1 : 0);
    }
    ss << std::setw(2) << static_cast<int>(byte);
  }

  logger->info(ss.str());
}

/// \brief Print little-endian uint32 from bits
/// \param bits The bit vector
/// \param bit_offset Starting bit index
/// \param name The name of the field
/// \param logger The spdlog logger instance
void printUint32LE(const std::vector<bool>& bits, size_t bit_offset,
                   const std::string& name,
                   std::shared_ptr<spdlog::logger>& logger) {
  if (bit_offset + 32 > bits.size()) {
    throw std::invalid_argument("Not enough bits for uint32");
  }

  uint32_t value = 0;
  for (int i = 0; i < 32; ++i) {
    value = (value << 1) | (bits[bit_offset + i] ? 1 : 0);
  }

  // Convert from bit representation to little-endian bytes
  uint8_t bytes[4];
  bytes[0] = value & 0xFF;
  bytes[1] = (value >> 8) & 0xFF;
  bytes[2] = (value >> 16) & 0xFF;
  bytes[3] = (value >> 24) & 0xFF;

  uint32_t le_value = static_cast<uint32_t>(bytes[0]) |
                      (static_cast<uint32_t>(bytes[1]) << 8) |
                      (static_cast<uint32_t>(bytes[2]) << 16) |
                      (static_cast<uint32_t>(bytes[3]) << 24);

  std::stringstream ss;
  ss << name << ": 0x" << std::hex << std::setfill('0') << std::setw(8)
     << le_value;
  logger->info(ss.str());
}

/// \brief Decode a raw Bitcoin block header (80 bytes hex string)
/// \param hex_header The 160-character hex string representing the block header
/// \param logger The spdlog logger instance
void decodeBlockHeader(const std::string& hex_header,
                       std::shared_ptr<spdlog::logger>& logger) {
  if (hex_header.length() != 160) {
    throw std::invalid_argument(
        "Block header must be exactly 160 hex characters (80 bytes)");
  }

  // Convert hex string to bits
  auto bits = hexStringToBits(hex_header);
  logger->info("Decoded {} bits from hex string", bits.size());

  if (bits.size() != 640) {
    throw std::invalid_argument(
        "Block header must be exactly 640 bits (80 bytes)");
  }

  logger->info("");
  logger->info("========== BITCOIN BLOCK HEADER PARSER ==========");
  logger->info("Raw Header (160 hex chars):");
  logger->info(hex_header);

  logger->info("========== PARSED COMPONENTS ==========");

  size_t bit_offset = 0;

  // Version (32 bits, little-endian)
  printUint32LE(bits, bit_offset, "Version", logger);
  bit_offset += 32;

  // Previous block hash (256 bits)
  printBitsReversed(bits, bit_offset, 256,
                    "Previous Block Hash (reversed for display)", logger);
  bit_offset += 256;

  // Merkle root (256 bits)
  printBitsReversed(bits, bit_offset, 256, "Merkle Root (reversed for display)",
                    logger);
  bit_offset += 256;

  // Timestamp (32 bits, little-endian)
  printUint32LE(bits, bit_offset, "Timestamp", logger);
  bit_offset += 32;

  // Bits (difficulty target, 32 bits, little-endian)
  printUint32LE(bits, bit_offset, "Bits (difficulty)", logger);
  bit_offset += 32;

  // Nonce (32 bits, little-endian)
  printUint32LE(bits, bit_offset, "Nonce", logger);
  bit_offset += 32;

  //   logger->info("\n========== FIELD DETAILS ==========");
  //   logger->info("Total header size: 80 bytes");
  //   logger->info("  - Version: 4 bytes (little-endian)");
  //   logger->info("  - Previous Block Hash: 32 bytes");
  //   logger->info("  - Merkle Root: 32 bytes");
  //   logger->info("  - Timestamp: 4 bytes (little-endian, Unix epoch)");
  //   logger->info("  - Bits: 4 bytes (little-endian, difficulty encoding)");
  //   logger->info("  - Nonce: 4 bytes (little-endian, proof-of-work value)");
  //   logger->info("");
}

int main(int argc, char* argv[]) {
  // Create a logger
  auto logger = spdlog::stdout_color_mt("blockheader_parser");
  logger->set_level(spdlog::level::info);
  spdlog::set_default_logger(logger);

  try {
    if (argc < 2) {
      logger->error("Usage: blockheader_parser <hex_string>");
      logger->error("   or: blockheader_parser --file <filename>");
      logger->error("");
      logger->error("Example:");
      logger->error("  blockheader_parser 00a019313345e37371616d...");
      return 1;
    }

    std::string input;

    if (argc >= 3 && std::string(argv[1]) == "--file") {
      // Read from file
      std::ifstream file(argv[2]);
      if (!file.is_open()) {
        logger->error("Error: Could not open file: {}", argv[2]);
        return 1;
      }

      // Read the first line containing the hex string
      std::getline(file, input);

      // Remove whitespace
      input.erase(std::remove_if(input.begin(), input.end(), ::isspace),
                  input.end());
    } else {
      input = argv[1];
      // Remove whitespace
      input.erase(std::remove_if(input.begin(), input.end(), ::isspace),
                  input.end());
    }

    decodeBlockHeader(input, logger);
    return 0;

  } catch (const std::exception& e) {
    logger->error("Error: {}", e.what());
    return 1;
  }
}
