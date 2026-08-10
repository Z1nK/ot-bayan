#pragma once

#include "crc32.hpp"
#include "hash.hpp"
#include "md5.hpp"

#include <stdexcept>
#include <string>

namespace bayan {

inline HashAlgorithm ParseHashAlgorithm(const std::string& name) {
  if (name == "crc32") {
    return HashAlgorithm::CRC32;
  }
  if (name == "md5") {
    return HashAlgorithm::MD5;
  }

  throw std::invalid_argument("Unsupported hash algorithm: " + name);
}

inline HashFunction MakeHashFunction(HashAlgorithm algorithm) {
  switch (algorithm) {
  case HashAlgorithm::CRC32:
    return
        [](const char* data, size_t size) -> HashValue { return {crc32(data, size), 0U, 0U, 0U}; };
  case HashAlgorithm::MD5:
    return 
        [](const char* data, size_t size) -> HashValue { return md5(data, size); };
  }

  throw std::invalid_argument("Unsupported hash algorithm");
}

}  // namespace bayan
