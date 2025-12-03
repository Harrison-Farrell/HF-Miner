// Google Test includes
#include <gtest/gtest.h>

// project includes
#include "util/endian.h"
#include <bit>

// Test toBigEndian with 64-bit values
TEST(EndianTest, toBigEndian) {
  uint64_t value = 0x123456789ABCDEF0;
  uint64_t result = util::toBigEndian(value);

  if constexpr (std::endian::native == std::endian::big) {
    EXPECT_EQ(result, 0x123456789ABCDEF0);
  } else {
    EXPECT_EQ(result, 0xF0DEBC9A78563412);
  }
}

// Test toLittleEndian with 64-bit values
TEST(EndianTest, toLittleEndian) {
  uint64_t value = 0x123456789ABCDEF0;
  uint64_t result = util::toLittleEndian(value);

  if constexpr (std::endian::native == std::endian::little) {
    EXPECT_EQ(result, 0x123456789ABCDEF0);
  } else {
    EXPECT_EQ(result, 0xF0DEBC9A78563412);
  }
}

// Test fromBigEndian with 64-bit values
TEST(EndianTest, fromBigEndian) {
  uint64_t value = 0x123456789ABCDEF0;
  uint64_t result = util::fromBigEndian(value);

  if constexpr (std::endian::native == std::endian::big) {
    EXPECT_EQ(result, 0x123456789ABCDEF0);
  } else {
    EXPECT_EQ(result, 0xF0DEBC9A78563412);
  }
}

// Test fromLittleEndian with 64-bit values
TEST(EndianTest, fromLittleEndian) {
  uint64_t value = 0x123456789ABCDEF0;
  uint64_t result = util::fromLittleEndian(value);

  if constexpr (std::endian::native == std::endian::little) {
    EXPECT_EQ(result, 0x123456789ABCDEF0);
  } else {
    EXPECT_EQ(result, 0xF0DEBC9A78563412);
  }
}

// Test roundtrip conversion for 64-bit: toBigEndian -> fromBigEndian
TEST(EndianTest, Roundtrip_64bit_BigEndian) {
  uint64_t original = 0x0123456789ABCDEF;
  uint64_t big_endian = util::toBigEndian(original);
  uint64_t restored = util::fromBigEndian(big_endian);

  EXPECT_EQ(restored, original);
}

// Test roundtrip conversion for 64-bit: toLittleEndian -> fromLittleEndian
TEST(EndianTest, Roundtrip_64bit_LittleEndian) {
  uint64_t original = 0x0123456789ABCDEF;
  uint64_t little_endian = util::toLittleEndian(original);
  uint64_t restored = util::fromLittleEndian(little_endian);

  EXPECT_EQ(restored, original);
}
