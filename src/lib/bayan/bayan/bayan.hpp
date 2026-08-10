#pragma once
#include <bayan/duplicate-finder/duplicate_finder.hpp>
#include <bayan/filesystem-helper/file_obj.hpp>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <cstdint>
#include <string>
#include <vector>


namespace bayan {

namespace fs = boost::filesystem;
namespace po = boost::program_options;
struct BayanOptions {
  std::vector<fs::path> scan_dirs;
  std::vector<fs::path> exclude_dirs;
  size_t depth;
  uint64_t min_size;
  std::vector<std::string> masks;
  size_t block_size;
  std::string hash_algorithm;
  bool show_help;
  bool relative_paths;
};

class Bayan {
public:
  Bayan();
  BayanOptions extractOptions(const po::variables_map& vm);
  void run(int argc, char* argv[]);

private:
  void printOptions(const BayanOptions& options) const;
  void printDuplicateGroups(const std::vector<FileObj>& files,
                             const std::vector<DuplicateFinder::Group>& groups,
                             bool relative_paths) const;
};

}  // namespace bayan