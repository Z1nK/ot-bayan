#pragma once

#include <bayan/filesystem-helper/file_obj.hpp>

#include <cstddef>
#include <vector>

namespace bayan {

// Groups files with identical content, comparing by size first and then by
// block hashes read progressively (so files that differ early are never
// read in full).
class DuplicateFinder {
public:
  
  using Group = std::vector<std::size_t>;

  // Returns groups of indices (size > 1) whose files are byte-for-byte
  // identical.
  std::vector<Group> Find(const std::vector<FileObj>& files) const;

private:
  std::vector<Group> GroupBySize(const std::vector<FileObj>& files) const;
  std::vector<Group> RefineGroupByHash(const std::vector<FileObj>& files, Group group) const;
};

}  // namespace bayan
