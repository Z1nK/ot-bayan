#pragma once
#include <bayan/hash/hash.hpp>

#include <boost/filesystem.hpp>

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <vector>

namespace bayan {
namespace fs = boost::filesystem;

class FileObj {
public:
  FileObj(const fs::path& path, uint64_t size, size_t blockSize);

  const fs::path& getPath() const;
  uint64_t getSize() const;
  size_t getBlockSize() const;

  // Note: Works only if the methods calls are made in order, i.e., first block 0, then block 1,
  // etc. could be problem if no order will be possible. (add std::optional .. ?)
  const HashValue& getBlockHash(size_t blockIndex) const;

private:
  fs::path path_;
  uint64_t size_;
  size_t blockSize_;

  HashValue readAndComputeHash(size_t blockIndex) const;

  mutable std::ifstream stream_;
  mutable std::vector<HashValue> cachedHashes_;
};

}  // namespace bayan