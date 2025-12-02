// system includes
#include <stdio.h>
#include <string.h>

// project includes
#include "block/blockHeader.h"
#include "sha256/sha256.h"
#include "types/types.h"

int main() {
  // Input text.
  const char *text = "Hello World!";

  /* Char array to store the hexadecimal SHA-256 string. */
  /* Must be 65 characters big (or larger). */
  /* The last character will be the null-character. */
  char hex[SHA256::SHA256_HEX_SIZE];

  // Compute SHA-256 sum.
  SHA256::sha256_hex(text, strlen(text), hex);

  // Print result.
  printf("The SHA-256 sum of \"%s\" is:\n\n", text);
  printf("%s\n\n", hex);

  return 0;
}