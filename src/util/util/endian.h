#ifndef __ENDIAN_H__
#define __ENDIAN_H__

// system includes
#include <cstdint>
#include <bit>

namespace util {

/// \brief  Convert a 16-bit integer to big-endian format.
/// \param value 16-bit integer to convert.
/// \return Big-endian representation of the input value.    
inline uint16_t toBigEndian(uint16_t value) {
    if constexpr (std::endian::native == std::endian::big) {
        return value;
    } else {
        return std::byteswap(value);
    }
}

/// \brief  Convert a 16-bit integer to little-endian format.
/// \param value 16-bit integer to convert.
/// \return Little-endian representation of the input value.
inline uint16_t toLittleEndian(uint16_t value) {
    if constexpr (std::endian::native == std::endian::little) {
        return value;
    } else {
        return std::byteswap(value);
    }
}

/// \brief  Convert a 16-bit integer from big-endian format to native format.
/// \param value 16-bit integer in big-endian format.
/// \return Native representation of the input value.
inline uint16_t fromBigEndian(uint16_t value) {
    if constexpr (std::endian::native == std::endian::big) {
        return value;
    } else {
        return std::byteswap(value);
    }
}

/// \brief  Convert a 16-bit integer from little-endian format to native format.
/// \param value 16-bit integer in little-endian format.
/// \return Native representation of the input value.
inline uint16_t fromLittleEndian(uint16_t value) {
    if constexpr (std::endian::native == std::endian::little) {
        return value;
    } else {
        return std::byteswap(value);
    }
}

/// \brief  Convert a 32-bit integer to big-endian format.
/// \param value 32-bit integer to convert.
/// \return Big-endian representation of the input value.
inline uint32_t toBigEndian(uint32_t value) {
    if constexpr (std::endian::native == std::endian::big) {
        return value;
    } else {
        return std::byteswap(value);
    }
}

/// \brief  Convert a 32-bit integer to little-endian format.
/// \param value 32-bit integer to convert.
/// \return Little-endian representation of the input value.
inline uint32_t toLittleEndian(uint32_t value) {
    if constexpr (std::endian::native == std::endian::little) {
        return value;
    } else {
        return std::byteswap(value);
    }
}

/// \brief  Convert a 32-bit integer from big-endian format to native format.
/// \param value 32-bit integer in big-endian format.
/// \return Native representation of the input value.
inline uint32_t fromBigEndian(uint32_t value) {
    if constexpr (std::endian::native == std::endian::big) {
        return value;
    } else {
        return std::byteswap(value);
    }
}

/// \brief  Convert a 32-bit integer from little-endian format to native format.
/// \param value 32-bit integer in little-endian format.
/// \return Native representation of the input value.
inline uint32_t fromLittleEndian(uint32_t value) {
    if constexpr (std::endian::native == std::endian::little) {
        return value;
    } else {
        return std::byteswap(value);
    }
}


/// \brief  Convert a 64-bit integer to big-endian format.
/// \param value 64-bit integer to convert.
/// \return Big-endian representation of the input value.
inline uint64_t toBigEndian(uint64_t value) {
    if constexpr (std::endian::native == std::endian::big) {
        return value;
    } else {
        return std::byteswap(value);
    }
}

/// \brief  Convert a 64-bit integer to little-endian format.
/// \param value 64-bit integer to convert.
/// \return Little-endian representation of the input value.
inline uint64_t toLittleEndian(uint64_t value) {
    if constexpr (std::endian::native == std::endian::little) {
        return value;
    } else {
        return std::byteswap(value);
    }
}

/// \brief  Convert a 64-bit integer from big-endian format to native format.
/// \param value 64-bit integer in big-endian format.
/// \return Native representation of the input value.
inline uint64_t fromBigEndian(uint64_t value) {
    if constexpr (std::endian::native == std::endian::big) {
        return value;
    } else {
        return std::byteswap(value);
    }
}

/// \brief  Convert a 64-bit integer from little-endian format to native format.
/// \param value 64-bit integer in little-endian format.
/// \return Native representation of the input value.
inline uint64_t fromLittleEndian(uint64_t value) {
    if constexpr (std::endian::native == std::endian::little) {
        return value;
    } else {
        return std::byteswap(value);
    }
}

} // namespace util

#endif // __ENDIAN_H__