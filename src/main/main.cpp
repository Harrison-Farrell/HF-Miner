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

  UI::runHelloWorld();

  // Print result.
  std::cout << std::format("The SHA-256 sum of \"{}\" is:\n\n", text);
  std::cout << std::format("{}", hex) << std::endl;

  UI::renderInputWindow();

  while (!g_shutdown_requested) {
    // Main loop can perform other tasks here
    // For demonstration, we'll just sleep for a short duration
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return EXIT_SUCCESS;
}