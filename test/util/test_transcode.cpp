// Google Test includes
#include <gtest/gtest.h>

// project includes
#include "util/transcode.h"

// Test ConstevalHexDigit with valid hex digits 0-9 and a-f
TEST(TranscodeTest, ConstevalHexDigit) {
  constexpr uint8_t result0 = util::ConstevalHexDigit('0');
  EXPECT_EQ(result0, 0);

  constexpr uint8_t result1 = util::ConstevalHexDigit('1');
  EXPECT_EQ(result1, 1);

  constexpr uint8_t result5 = util::ConstevalHexDigit('5');
  EXPECT_EQ(result5, 5);

  constexpr uint8_t result9 = util::ConstevalHexDigit('9');
  EXPECT_EQ(result9, 9);

    constexpr uint8_t resulta = util::ConstevalHexDigit('a');
  EXPECT_EQ(resulta, 0x0A);

  constexpr uint8_t resultb = util::ConstevalHexDigit('b');
  EXPECT_EQ(resultb, 0x0B);

  constexpr uint8_t resultc = util::ConstevalHexDigit('c');
  EXPECT_EQ(resultc, 0x0C);

  constexpr uint8_t resultd = util::ConstevalHexDigit('d');
  EXPECT_EQ(resultd, 0x0D);

  constexpr uint8_t resulte = util::ConstevalHexDigit('e');
  EXPECT_EQ(resulte, 0x0E);

  constexpr uint8_t resultf = util::ConstevalHexDigit('f');
  EXPECT_EQ(resultf, 0x0F);
}
