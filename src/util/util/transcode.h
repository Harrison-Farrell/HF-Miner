#ifndef __TRANSCODE_H__
#define __TRANSCODE_H__

// system includes
#include <cstdint>

namespace util {
/// \brief Convert a single hexadecimal character to its integer value at compile time.
/// \param c Hexadecimal character ('0'-'9', 'a'-'f')
/// \return Integer value (0-15) corresponding to the hex character.
/// \note Only lowercase hex digits are supported for consistency.
consteval uint8_t ConstevalHexDigit(const char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 0xa;

    throw "Only lowercase hex digits are allowed, for consistency";
}

} // namespace util

#endif // __TRANSCODE_H__