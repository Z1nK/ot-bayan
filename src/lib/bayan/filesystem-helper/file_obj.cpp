#include "file_obj.hpp"

#include <bayan/hash/hash_factory.hpp>

#include <vector>

namespace bayan {

FileObj::FileObj(const fs::path& path, uint64_t size, size_t blockSize, HashFunction hashFunction)
    : path_(path), size_(size), blockSize_(blockSize), hashFunction_(std::move(hashFunction)) {}

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
  (void)blockIndex;  // relies on sequential access, see class note above

  if (!stream_.is_open()) {
    stream_.open(path_.string(), std::ios::binary);
  }

  std::vector<char> buffer(blockSize_, 0);
  stream_.read(buffer.data(), static_cast<std::streamsize>(blockSize_));
  auto bytesRead = static_cast<size_t>(stream_.gcount());

  return hashFunction_(buffer.data(), bytesRead);
}

}  // namespace bayan
