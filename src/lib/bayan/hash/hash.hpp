#pragma once
#include <functional>
#include <array>
#include <cstdint>

namespace bayan {

using HashValue = std::array<uint32_t, 4>;
using HashFunction = std::function<HashValue(const char*, size_t)>;

enum class HashAlgorithm {
  CRC32,
  MD5
};

}  // namespace bayan