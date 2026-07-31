#pragma once

#include <boost/filesystem.hpp>
#include <boost/functional/hash.hpp>

#include <cstdint>
#include <regex>
#include <string>
#include <unordered_set>
#include <vector>

namespace bayan {

namespace fs = boost::filesystem;

class FileFinder {
public:
  FileFinder() = default;

  // --- Direct Scan Directories ---
  FileFinder& AddScanDir(const fs::path& dir_path);

  FileFinder& AddScanDir(const std::vector<fs::path>& dir_paths);

  // --- Exclude Directories ---
  // Bare name (e.g. ".git") excludes any directory with that name, anywhere.
  FileFinder& AddExcludeDir(const std::string& dir_name);

  FileFinder& AddExcludeDir(const std::vector<std::string>& dir_names);

  // Concrete path excludes only that specific directory.
  FileFinder& AddExcludeDir(const fs::path& dir_path);

  FileFinder& AddExcludeDir(const std::vector<fs::path>& dir_paths);

  // --- Masks ---
  FileFinder& AddMask(const std::string& wildcard_pattern);

  FileFinder& AddMask(const std::vector<std::string>& wildcard_patterns);

  // --- Settings ---
  FileFinder& SetScanDepth(std::size_t depth);

  FileFinder& SetMinFileSize(std::uint64_t min_bytes);

  // --- Execution ---
  std::vector<fs::path> Find() const;

private:
  std::vector<fs::path> scan_dirs_;
  std::unordered_set<std::string> exclude_dir_names_;
  std::unordered_set<fs::path, boost::hash<fs::path>> exclude_paths_;
  std::vector<std::regex> mask_regexes_;

  std::size_t scan_depth_ = 0;
  std::uint64_t min_file_size_bytes_ = 0;

  bool MatchesMasks(const std::string& filename) const;

  bool IsExcludedDir(const fs::path& dir_path) const;

  bool SatisfiesFileSize(const fs::path& file_path) const;

  static std::string WildcardToRegex(const std::string& pattern);

  void ScanDirectory(const fs::path& root_path, std::vector<fs::path>& result) const;
};

}  // namespace bayan