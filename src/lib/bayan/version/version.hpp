#pragma once
#include <string>
#include <tuple>

#include <version_constants.hpp>

namespace bayan {

inline std::string version_string() { return kProjectVersionString; }

inline std::tuple<int, int, int> version() {
    return {kProjectVersionMajor, kProjectVersionMinor, kProjectVersionPatch};
}

}  // namespace bayan