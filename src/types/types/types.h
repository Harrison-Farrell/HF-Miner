#ifndef __TYPES_H__
#define __TYPES_H__

// system includes
#include <array>
#include <bitset>

/// \typedef Hash
/// \brief A 32-byte array representing a cryptographic hash.
/// \details Used to store SHA-256 hash values and other 32-byte identifiers
/// such as block hashes, transaction hashes, and merkle roots in the blockchain.
typedef std::array<unsigned char, 32> Hash;

/// \typedef BigInt256
/// \brief A 256-bit big integer representation using std::bitset.
/// \details This type is useful for handling large integers such as
/// difficulty targets in blockchain applications.
typedef std::bitset<256> ;

/// \typedef BigInt32
/// \brief A 32-bit big integer representation using std::bitset.
/// \details This type can be used for smaller integer values where
typedef std::bitset<32> fourbytes;


#endif // __TYPES_H__