#include "duplicate_finder.hpp"

#include <algorithm>
#include <unordered_map>

namespace bayan {

namespace {

struct HashValueHasher {
  std::size_t operator()(const HashValue& value) const {
    std::size_t seed = 0;
    for (uint32_t part : value) {
      seed ^= std::hash<uint32_t>{}(part) + 0x9e3779b9U + (seed << 6) + (seed >> 2);      
      // seed ^= std::hash<uint32_t>{}(part) + 0x9e3779b97f4a7c15ULL + (seed << 12) + (seed >> 4);
    }
    return seed;
  }
};

}  // namespace

std::vector<DuplicateFinder::Group> DuplicateFinder::Find(const std::vector<FileObj>& files) const {
  std::vector<Group> result;

  for (auto& size_group : GroupBySize(files)) {
    for (auto& duplicate_group : RefineGroupByHash(files, std::move(size_group))) {
      result.push_back(std::move(duplicate_group));
    }
  }

  return result;
}

std::vector<DuplicateFinder::Group> DuplicateFinder::GroupBySize(
    const std::vector<FileObj>& files) const {
  std::unordered_map<std::uint64_t, Group> groups_by_size;

  for (std::size_t i = 0; i < files.size(); ++i) {
    groups_by_size[files[i].getSize()].push_back(i);
  }

  std::vector<Group> groups;
  for (auto& [size, group] : groups_by_size) {
    if (group.size() > 1) {
      groups.push_back(std::move(group));
    }
  }
  return groups;
}

std::vector<DuplicateFinder::Group> DuplicateFinder::RefineGroupByHash(
    const std::vector<FileObj>& files, Group group) const {
  // All files in `group` share the same size and block size by construction.
  const std::uint64_t file_size = files[group.front()].getSize();
  const std::size_t block_size = std::max<std::size_t>(files[group.front()].getBlockSize(), 1);
  const std::size_t total_blocks =
      static_cast<std::size_t>((file_size + block_size - 1) / block_size);

  std::vector<Group> active_groups;
  active_groups.push_back(std::move(group));

  for (std::size_t block_index = 0; block_index < total_blocks && !active_groups.empty();
       ++block_index) {
    std::vector<Group> next_active_groups;

    for (const auto& active_group : active_groups) {
      std::unordered_map<HashValue, Group, HashValueHasher> buckets;
      for (std::size_t file_index : active_group) {
        buckets[files[file_index].getBlockHash(block_index)].push_back(file_index);
      }

      for (auto& [hash, bucket] : buckets) {
        if (bucket.size() > 1) {
          next_active_groups.push_back(std::move(bucket));
        }
      }
    }

    active_groups = std::move(next_active_groups);
  }

  return active_groups;
}

}  // namespace bayan
