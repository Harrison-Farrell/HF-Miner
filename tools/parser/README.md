# Bitcoin Block Header Parser

A command-line utility to decode and display the components of a raw Bitcoin block header.

## Overview

This tool parses a 160-character hexadecimal string (representing 80 bytes) and displays all components of a Bitcoin block header in a human-readable format.

## Block Header Structure

A Bitcoin block header consists of 6 fields totaling 80 bytes:

| Field | Size | Type | Description |
|-------|------|------|-------------|
| Version | 4 bytes | Little-endian | Block version number |
| Previous Block Hash | 32 bytes | Raw bytes | Hash of the previous block |
| Merkle Root | 32 bytes | Raw bytes | Root hash of the transaction tree |
| Timestamp | 4 bytes | Little-endian | Block creation time (Unix epoch) |
| Bits | 4 bytes | Little-endian | Difficulty target encoding |
| Nonce | 4 bytes | Little-endian | Proof-of-work value |

## Usage

### From command line with hex string:
```bash
blockheader_parser 00a019313345e37371616dff03885cd2811146c3a5044ae4aee301000000000000000000b4f285b216a082b1381d17530724332868561b46af7c9d8636d875f273557864cc7efe6288ed091721d60f80
```

### From a file:
```bash
blockheader_parser --file <path/to/file.txt>
```

The file should contain a single line with the 160-character hexadecimal string.

## Output

The parser displays:
- **Raw Header**: The full 160-character hex string for reference
- **Parsed Components**: Each field decoded and displayed in appropriate format:
  - Scalar fields (version, timestamp, bits, nonce) shown as hex and decimal
  - Hash fields shown in reversed byte order (as is standard in Bitcoin)
- **Field Details**: Summary of the header structure and field byte orders

## Example Output

```
========== BITCOIN BLOCK HEADER PARSER ==========

Raw Header (160 hex chars):
00a019313345e37371616dff03885cd2811146c3a5044ae4aee301000000000000000000b4f285b216a082b1381d17530724332868561b46af7c9d8636d875f273557864cc7efe6288ed091721d60f80

========== PARSED COMPONENTS ==========
Version: 0x3119a000
Previous Block Hash (reversed for display): 00000000000000000001e3aee44a04a5c3461181d25c8803ff6d617173e34533
Merkle Root (reversed for display): 64785573f275d836869d7caf461b56682833240753171d38b182a016b285f2b4
Timestamp: 0x62fe7ecc (1660845772 seconds since epoch)
Bits (difficulty): 0x1709ed88
Nonce: 0x800fd621

========== FIELD DETAILS ==========
Total header size: 80 bytes
  - Version: 4 bytes (little-endian)
  - Previous Block Hash: 32 bytes
  - Merkle Root: 32 bytes
  - Timestamp: 4 bytes (little-endian, Unix epoch)
  - Bits: 4 bytes (little-endian, difficulty encoding)
  - Nonce: 4 bytes (little-endian, proof-of-work value)
```

## Building

The parser is built as part of the HF-Miner project build system:

```bash
cd build
cmake --build . --config Debug
```

The executable will be located at: `build/tools/parser/Debug/blockheader_parser.exe`

## Error Handling

The tool validates input and provides clear error messages:
- Invalid hex characters
- Incorrect header length (must be exactly 160 hex characters / 80 bytes)
- File not found errors
- Argument validation

## Notes

- Hex strings can contain optional whitespace which will be stripped
- Hash fields are displayed in reversed (big-endian) byte order as is standard in Bitcoin explorers
- Timestamp values are shown in both hex and human-readable Unix epoch format
- The parser is a standalone utility with no external dependencies beyond the standard library
