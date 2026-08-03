#include "file_obj.hpp"

#include <bayan/hash/md5.hpp>

#include <vector>

namespace bayan {

FileObj::FileObj(const fs::path& path, uint64_t size, size_t blockSize)
    : path_(path), size_(size), blockSize_(blockSize) {}

const fs::path& FileObj::getPath() const {
  return path_;
}

uint64_t FileObj::getSize() const {
  return size_;
}

size_t FileObj::getBlockSize() const {
  return blockSize_;
}

const HashValue& FileObj::getBlockHash(size_t blockIndex) const {
  if (blockIndex >= cachedHashes_.size()) {
    cachedHashes_.resize(blockIndex + 1);
    cachedHashes_[blockIndex] = readAndComputeHash(blockIndex);
  }

  return cachedHashes_[blockIndex];
}

HashValue FileObj::readAndComputeHash(size_t blockIndex) const {
    //TODO Add hash computation logic here. For now, we return a placeholder value.
    return {0U, 0U, 0U, 0U}; // Placeholder for actual hash computation logic
}

}  // namespace bayan
