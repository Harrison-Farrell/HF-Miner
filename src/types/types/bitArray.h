#ifndef __BITARRAY_H__
#define __BITARRAY_H__

// system includes
#include <algorithm>
#include <array>
#include <cassert>
#include <cstring>
#include <span>
#include <string_view>

// project includes
#include "util/endian.h"
#include "util/transcode.h"

template <unsigned int BITS> class BitArray {
protected:
  // Ensure that BITS is a positive multiple of 8
  static constexpr unsigned int mBytes = BITS / 8;
  static_assert(BITS % 8 == 0,
                "BitArray can only store bytes. BITS must be a multiple of 8");
  std::array<uint8_t, mBytes> mdata;
  static_assert(mBytes == sizeof(mdata),
                "BitArray must have at least one byte");

public:
  /// \brief Default constructor initializes all bits to zero.
  constexpr BitArray() : mdata() {}

  /// \brief Constructor from a single byte value.
  /// \param byte Byte value to initialize the BitArray.
  constexpr explicit BitArray(uint8_t byte) : mdata{byte} {}

  /// \brief Constructor from a span of bytes.
  /// \param bytes Span of bytes to initialize the BitArray.
  constexpr explicit BitArray(std::span<const unsigned char> char_bytes) {
    // Ensure input span size matches BitArray size
    assert(char_bytes.size() == mBytes &&
           "Input byte span size must match BitArray size");
    std::copy(char_bytes.begin(), char_bytes.end(), mdata.begin());
  }

  /// \brief Constructor from a hexadecimal string.
  /// \param hex_str Hexadecimal string representation of bits to initialize the
  /// BitArray.
  consteval explicit BitArray(std::string_view hex_str);

  /// @brief  Compare this BitArray with another. Lexicographic ordering
  /// @param other bit array to compare against
  /// @return zero if equal, negative if this < other, positive if this > other
  constexpr int Compare(const BitArray &other) const {
    return std::memcmp(mdata.data(), other.mdata.data(), mBytes);
  }

  /// \brief Check if all bits are zero.
  /// \return true if all bits are zero, false otherwise.
  constexpr bool IsNull() const {
    return std::all_of(mdata.begin(), mdata.end(),
                       [](uint8_t val) { return val == 0; });
  }

  /// \brief Set all bits to zero.
  constexpr void SetNull() { std::fill(mdata.begin(), mdata.end(), 0); }

  constexpr const unsigned char *data() const { return mdata.data(); }
  constexpr unsigned char *data() { return mdata.data(); }
  constexpr unsigned char *begin() { return mdata.data(); }
  constexpr unsigned char *end() { return mdata.data() + mBytes; }
  constexpr const unsigned char *begin() const { return mdata.data(); }
  constexpr const unsigned char *end() const { return mdata.data() + mBytes; }
  static constexpr unsigned int size() { return mBytes; }
  constexpr uint64_t GetUint64(int pos) const {
    return ReadLE64(mdata.data() + pos * 8);
  }
};

template <unsigned int BITS>
consteval BitArray<BITS>::BitArray(std::string_view hex_str) {
  if (hex_str.length() != mdata.size() * 2)
    throw "Hex string must fit exactly";
  auto str_it = hex_str.rbegin();
  for (auto &elem : mdata) {
    auto lo = util::ConstevalHexDigit(*(str_it++));
    elem = (util::ConstevalHexDigit(*(str_it++)) << 4) | lo;
  }
}

#endif // __BITARRAY_H__
