#pragma once
#include <boost/filesystem.hpp>

namespace bayan {
namespace fs = boost::filesystem;

class FileObj {
public:
  FileObj(const fs::path& path, uint64_t size, size_t blockSize)
      : path_(path), size_(size), blockSize_(blockSize) {}

  const fs::path& getPath() const { return path_; }
  uint64_t getSize() const { return size_; }
  size_t getBlockSize() const { return blockSize_; }

  // TODO : Add methods for reading files and compute hashes.

private:
  fs::path path_;
  uint64_t size_;
  size_t blockSize_;
};

}  // namespace bayan