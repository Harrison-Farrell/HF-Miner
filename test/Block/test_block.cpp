// Google Test includes
#include <gtest/gtest.h>

// project includes
#include <Block/block.h>

TEST(BlockTEST, convertLittleEndianToBigEndian) {
  uint32_t value = 0x1A2B3C4D;
  uint32_t expected = 0x4D3C2B1A;

  Block::Block block;
  block.convertLittleEndianToBigEndian(value);

  EXPECT_EQ(value, expected);
}

TEST(BlockTEST, convertBigEndianToLittleEndian) {
  uint32_t value = 0x1A2B3C4D;
  uint32_t expected = 0x4D3C2B1A;

  Block::Block block;
  block.convertBigEndianToLittleEndian(value);

  EXPECT_EQ(value, expected);
}