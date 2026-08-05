#include "file_finder.hpp"

namespace bayan {

FileFinder& FileFinder::AddScanDir(const fs::path& dir_path) {
  scan_dirs_.push_back(dir_path);
  return *this;
}

FileFinder& FileFinder::AddScanDir(const std::vector<fs::path>& dir_paths) {
  for (const auto& dir : dir_paths) {
    AddScanDir(dir);
  }
  return *this;
}

FileFinder& FileFinder::AddExcludeDir(const std::string& dir_name) {
  exclude_dir_names_.insert(dir_name);
  return *this;
}

FileFinder& FileFinder::AddExcludeDir(const std::vector<std::string>& dir_names) {
  for (const auto& dir : dir_names) {
    AddExcludeDir(dir);
  }
  return *this;
}

FileFinder& FileFinder::AddExcludeDir(const fs::path& dir_path) {
  boost::system::error_code ec;
  fs::path canonical_p = fs::canonical(dir_path, ec);
  exclude_paths_.insert(ec ? dir_path : canonical_p);
  return *this;
}

FileFinder& FileFinder::AddExcludeDir(const std::vector<fs::path>& dir_paths) {
  for (const auto& dir : dir_paths) {
    AddExcludeDir(dir);
  }
  return *this;
}

FileFinder& FileFinder::AddMask(const std::string& wildcard_pattern) {
  if (!wildcard_pattern.empty()) {
    mask_regexes_.emplace_back(WildcardToRegex(wildcard_pattern), std::regex::icase);
  }
  return *this;
}

FileFinder& FileFinder::AddMask(const std::vector<std::string>& wildcard_patterns) {
  for (const auto& mask : wildcard_patterns) {
    AddMask(mask);
  }
  return *this;
}

FileFinder& FileFinder::SetScanDepth(std::size_t depth) {
  scan_depth_ = depth;
  return *this;
}

FileFinder& FileFinder::SetBlockSize(std::size_t block_size) {
  block_size_ = block_size;
  return *this;
}

FileFinder& FileFinder::SetMinFileSize(std::uint64_t min_bytes) {
  min_file_size_bytes_ = min_bytes;
  return *this;
}

std::vector<FileObj> FileFinder::Find() const {
  std::vector<FileObj> result;

  for (const auto& root_path : scan_dirs_) {
    boost::system::error_code ec;
    if (!fs::exists(root_path, ec) || !fs::is_directory(root_path, ec)) {
      continue;
    }
    ScanDirectory(root_path, result);
  }

  return result;
}

bool FileFinder::MatchesMasks(const std::string& filename) const {
  if (mask_regexes_.empty())
    return true;

  for (const auto& regex_pattern : mask_regexes_) {
    if (std::regex_match(filename, regex_pattern)) {
      return true;
    }
  }
  return false;
}

bool FileFinder::IsExcludedDir(const fs::path& dir_path) const {
  std::string folder_name = dir_path.filename().string();
  if (exclude_dir_names_.find(folder_name) != exclude_dir_names_.end()) {
    return true;
  }

  boost::system::error_code ec;
  fs::path canonical_p = fs::canonical(dir_path, ec);
  fs::path target_path = ec ? dir_path : canonical_p;

  return exclude_paths_.find(target_path) != exclude_paths_.end();
}

bool FileFinder::SatisfiesFileSize(const fs::path& file_path) const {
  if (min_file_size_bytes_ == 0)
    return true;

  boost::system::error_code ec;
  std::uint64_t file_size = fs::file_size(file_path, ec);
  if (ec)
    return false;

  return file_size >= min_file_size_bytes_;
}

std::string FileFinder::WildcardToRegex(const std::string& pattern) {
  std::string result = "^";
  for (char ch : pattern) {
    switch (ch) {
    case '*':
      result += ".*";
      break;
    case '?':
      result += ".";
      break;
    case '.':
    case '\\':
    case '+':
    case '^':
    case '$':
    case '(':
    case ')':
    case '[':
    case ']':
    case '{':
    case '}':
    case '|':
      result += '\\';
      result += ch;
      break;
    default:
      result += ch;
      break;
    }
  }
  result += "$";
  return result;
}

void FileFinder::ScanDirectory(const fs::path& root_path, std::vector<FileObj>& result) const {
  boost::system::error_code ec;
  fs::recursive_directory_iterator it(root_path, fs::directory_options::none, ec), end;

  while (it != end) {
    const fs::path& current_path = it->path();
    std::size_t current_depth = static_cast<std::size_t>(it.depth());

    if (fs::is_directory(current_path, ec)) {
      if (IsExcludedDir(current_path) || current_depth >= scan_depth_) {
        it.disable_recursion_pending();
      }
    } else if (fs::is_regular_file(current_path, ec)) {
      if (MatchesMasks(current_path.filename().string()) && SatisfiesFileSize(current_path)) {
        // Assuming a block size of 4096 bytes
        result.push_back(FileObj(current_path, fs::file_size(current_path, ec), block_size_));
      }
    }

    it.increment(ec);
    if (ec) {
      ec.clear();
    }
  }
}

}  // namespace bayan
