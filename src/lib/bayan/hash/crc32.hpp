#pragma once

#include <boost/crc.hpp>

namespace bayan {

inline uint32_t crc32(const char* data, size_t size) {
  boost::crc_32_type result;
  result.process_bytes(data, size);
  return result.checksum();
}

}  // namespace bayan