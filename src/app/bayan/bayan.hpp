#pragma once
#include <boost/program_options.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace po = boost::program_options;
namespace bayan {

struct BayanOptions {
  std::vector<std::string> scan_dirs;
  std::vector<std::string> exclude_dirs;
  size_t depth;
  uint64_t min_size;
  std::vector<std::string> masks;
  size_t block_size;
  std::string hash_algorithm;
  bool show_help;
};

class Bayan {
public:
  Bayan();
  BayanOptions extractOptions(const po::variables_map& vm);
  void run(int argc, char* argv[]);

private:
  void printOptions(const BayanOptions& options) const;
};

}  // namespace bayan