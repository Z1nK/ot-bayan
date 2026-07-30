#pragma once

#include <array>
#include <cstdint>
#include <boost/uuid/detail/md5.hpp>

namespace bayan {

std::array<uint32_t, 4> md5(const char* data, size_t size) {
  boost::uuids::detail::md5 hash;
  hash.process_bytes(data, size);
  boost::uuids::detail::md5::digest_type digest;
  hash.get_digest(digest);

  return {digest[0], digest[1], digest[2], digest[3]};
}

}  // namespace bayan