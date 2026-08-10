#pragma once
#include <version_constants.hpp>

#include <string>
#include <tuple>

namespace bayan {

inline std::string version_string() {
  return kProjectVersionString;
}

inline std::tuple<int, int, int> version() {
  return {kProjectVersionMajor, kProjectVersionMinor, kProjectVersionPatch};
}

}  // namespace bayan