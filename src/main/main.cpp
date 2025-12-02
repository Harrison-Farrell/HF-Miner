// system includes
#include <cstdlib>
#include <csignal>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <thread>
#include <chrono>

// project includes
#include "block/blockHeader.h"
#include "sha256/sha256.h"
#include "types/types.h"
#include "ui/homepage.h"

// Global flag for graceful shutdown
static volatile bool g_shutdown_requested = false;

/// \brief Signal handler for interrupt signals (SIGINT, SIGTERM)
/// \param signal The signal number received
void signalHandler(int signal) {
  if (signal == SIGINT || signal == SIGTERM) {
    g_shutdown_requested = true;
    std::cout << "\nShutdown signal received. Exiting gracefully...\n";
  }
}

int main() {
  // Register signal handlers
  std::signal(SIGINT, signalHandler);   // Handle Ctrl+C
  std::signal(SIGTERM, signalHandler);  // Handle termination signal

  // Input text.
  std::string text = "Hello World!";

  /* Char array to store the hexadecimal SHA-256 string. */
  /* Must be 65 characters big (or larger). */
  /* The last character will be the null-character. */
  char hex[SHA256::SHA256_HEX_SIZE];

  // Compute SHA-256 sum.
  SHA256::sha256_hex(text.c_str(), text.size(), hex);

  // Print result.
  std::cout << std::format("The SHA-256 sum of \"{}\" is:\n\n", text);
  std::cout << std::format("{}", hex) << std::endl;

  // Example: Create a block header and find a valid nonce
  std::cout << "\n" << std::string(60, '=') << "\n";
  std::cout << "Block Header Nonce Finding Example\n";
  std::cout << std::string(60, '=') << "\n\n";

  Block::BlockHeader exampleBlock;
  exampleBlock.setVersion(4);
  exampleBlock.setTimestamp(1672700353);
  exampleBlock.setBits(0x1707f590);  // Difficulty target bits 00000000000005a16b0000000000000000000000000000000000000000000000
  exampleBlock.setPrevBlockHash(SHA256::hashStringToArray("00000000000000000002a42bbc873b4423d9bf3a41a470b6876f7eb5cfebdc8e"));
  exampleBlock.setMerkleRoot(SHA256::hashStringToArray("041178edb820e2c9447166bdda63de11016bcf8827acc15bb3f2b30ef0c7aa70"));
  exampleBlock.setNonce(1535938367);  // Attempting to reach 1535938367

  // Initialize a 256-bit target (e.g., using a custom large integer class or array of bytes)
  // For simplicity, let's represent it as a byte array for demonstration.
  unsigned char target[32] = {0}; 
  unsigned int bits = exampleBlock.getBits(); // Example bits value
  unsigned char exponent = (bits >> 24) & 0xFF;
  unsigned int coefficient = bits & 0xFFFFFF;

  // Calculate the number of bytes to shift the coefficient
  int shiftBytes = exponent - 3; // Coefficient is 3 bytes, exponent is for the full number

  // Place the coefficient into the target array
  // Assuming little-endian representation for the coefficient bytes
  target[shiftBytes + 2] = (coefficient >> 16) & 0xFF;
  target[shiftBytes + 1] = (coefficient >> 8) & 0xFF;
  target[shiftBytes + 0] = coefficient & 0xFF; 

  std::cout << "Searching for valid nonce...\n";
  std::cout << std::format("Target difficulty: {}\n", target);
  std::cout << std::format("Version: 4, Timestamp: 1764656579, Bits: 0x1a083cc9\n\n");

  // Search for valid nonce with progress printing
  uint32_t foundNonce = 0;
  bool nonceFound = false;
  uint32_t printInterval = 1000000;  // Print every 1 million attempts

  std::cout << std::format("Initial block hash: {}\n", exampleBlock.calculateBlockHash());

  for (uint32_t attempt =  0; attempt < 50000000 && !g_shutdown_requested; ++attempt) {
    Hash currentHash = exampleBlock.incrementNonceAndHash();

    // Print progress every printInterval attempts
    if (attempt % printInterval == 0 && attempt > 0) {
      std::cout << std::format("Attempt {:>10}: Nonce = {:<10} Hash = {}\n",
                               attempt,
                               exampleBlock.getNonce(),
                               SHA256::hashArrayToString(currentHash));
    }

    // Check if hash meets difficulty target
    bool valid = true;
    for (int i = static_cast<int>(currentHash.size()) - 1; i >= 0; --i) {
      if (currentHash[i] < target[i]) {
        foundNonce = exampleBlock.getNonce();
        nonceFound = true;
        std::cout << std::format("FOUND! Nonce = {} after {} attempts\n", foundNonce, attempt + 1);
        std::cout << std::format("Valid hash: {}\n\n", SHA256::hashArrayToString(currentHash));
        break;
      } else if (currentHash[i] > target[i]) {
        break;
      }
    }

    if (nonceFound) break;
  }

  if (!nonceFound) {
    std::cout << "\nNo valid nonce found in this search range.\n\n";
  }

  while (!g_shutdown_requested) {
    // Main loop can perform other tasks here
    // For demonstration, we'll just sleep for a short duration
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return EXIT_SUCCESS;
}