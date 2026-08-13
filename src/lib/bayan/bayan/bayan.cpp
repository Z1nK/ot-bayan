#include "bayan.hpp"

#include <bayan/cli-parser/cli-parser.hpp>
#include <bayan/filesystem-helper/file_finder.hpp>
#include <bayan/filesystem-helper/file_obj.hpp>
#include <bayan/hash/hash_factory.hpp>

#include <bayan/version/version.hpp>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <type_traits>
#include <unordered_map>

namespace bayan {

Bayan::Bayan() {}

BayanOptions Bayan::extractOptions(const po::variables_map& vm) {
  BayanOptions options;

  options.show_help = vm.count("help") > 0;

  if (vm.count("scan")) {
    const auto& scan_dirs = vm["scan"].as<std::vector<std::string>>();
    options.scan_dirs.assign(scan_dirs.begin(), scan_dirs.end());
  }

  if (vm.count("exclude")) {
    const auto& exclude_dirs = vm["exclude"].as<std::vector<std::string>>();
    options.exclude_dirs.assign(exclude_dirs.begin(), exclude_dirs.end());
  }

  options.depth = vm["depth"].as<size_t>();
  options.min_size = vm["min-size"].as<uint64_t>();

  if (vm.count("mask")) {
    options.masks = vm["mask"].as<std::vector<std::string>>();
  }

  options.block_size = vm["block-size"].as<size_t>();
  options.hash_algorithm = vm["hash"].as<std::string>();
  options.relative_paths = vm.count("relative") > 0;
  options.verbose = vm.count("verbose") > 0;

  return options;
}

namespace {

template <typename T>
std::string Join(const std::vector<T>& values) {
  if (values.empty()) {
    return "-";
  }
  std::string joined;
  for (const auto& value : values) {
    if (!joined.empty()) {
      joined += ", ";
    }
    if constexpr (std::is_same_v<T, fs::path>) {
      joined += value.string();
    } else {
      joined += value;
    }
  }
  return joined;
}

constexpr int kParamColumnWidth = 20;
constexpr int kValueColumnWidth = 40;

void PrintTableRow(const std::string& param, const std::string& value,
                    int first_column_width = kParamColumnWidth,
                    int second_column_width = kValueColumnWidth) {
  std::cout << std::left << std::setw(first_column_width) << param
             << std::setw(second_column_width) << value << "\n";
}

}  // namespace

void Bayan::printOptions(const BayanOptions& options) const {
  std::cout << "Bayan - File Duplicate Finder\n";
  std::cout << std::string(kParamColumnWidth + kValueColumnWidth, '=') << "\n";

  PrintTableRow("Parameter", "Value");
  std::cout << std::string(kParamColumnWidth + kValueColumnWidth, '-') << "\n";

  PrintTableRow("Scan directories", Join(options.scan_dirs));
  PrintTableRow("Exclude directories", Join(options.exclude_dirs));
  PrintTableRow("Scan depth", std::to_string(options.depth));
  PrintTableRow("Minimum file size", std::to_string(options.min_size) + " bytes");
  PrintTableRow("File masks", Join(options.masks));
  PrintTableRow("Block size", std::to_string(options.block_size));
  PrintTableRow("Hash algorithm", options.hash_algorithm);
  PrintTableRow("Relative paths", options.relative_paths ? "yes" : "no");

  std::cout << std::endl;
}

void Bayan::run(int argc, char* argv[]) {
  CliParser parser;
  po::variables_map vm = parser.parse(argc, argv);

  if (vm.count("help")) {
    parser.printHelp();
    return;
  }

  if (vm.count("version")) {
    std::cout << bayan::version_string() << std::endl;
    return;
  }

  BayanOptions options = extractOptions(vm);

  if (options.scan_dirs.empty()) {
    std::cerr << "Error: At least one scan directory must be specified with --scan" << std::endl;
    parser.printHelp();
    return;
  }

  if (options.verbose) {
    printOptions(options);
  }

  FileFinder finder;
  finder.AddScanDir(options.scan_dirs)
      .AddExcludeDir(options.exclude_dirs)
      .SetScanDepth(options.depth)
      .SetMinFileSize(options.min_size)
      .AddMask(options.masks)
      .SetBlockSize(options.block_size)
      .SetHashAlgorithm(ParseHashAlgorithm(options.hash_algorithm));

  std::vector<FileObj> files = finder.Find();

  DuplicateFinder duplicate_finder;
  std::vector<DuplicateFinder::Group> duplicate_groups = duplicate_finder.Find(files);

  printDuplicateGroups(files, duplicate_groups, options);
}

std::string Bayan::resolvePath(const fs::path& path, bool relative_paths) {
  if (relative_paths) {
    return path.string();
  }

  boost::system::error_code ec;
  fs::path full_path = fs::canonical(path, ec);
  if (!ec) {
    return full_path.string();
  }

  full_path = fs::absolute(path, ec);
  return (ec ? path : full_path).string();
}

void Bayan::printDuplicateGroups(const std::vector<FileObj>& files,
                                  const std::vector<DuplicateFinder::Group>& groups,
                                  const BayanOptions& options) const {
  if (groups.empty()) {
    return;
  }

  if (options.verbose) {
    std::cout << "Found " << groups.size() << " group(s) of duplicates:\n";
  }

  std::unordered_map<size_t, std::string> resolved_paths;
  auto getResolvedPath = [&resolved_paths, &files, &options](size_t file_index) -> const std::string& {
    auto [it, inserted] = resolved_paths.try_emplace(file_index);
    if (inserted) {
      it->second = resolvePath(files[file_index].getPath(), options.relative_paths);
    }
    return it->second;
  };

  int path_column_width = kValueColumnWidth;
  if (options.verbose) {
    for (const auto& group : groups) {
      for (size_t file_index : group) {
        int path_length = static_cast<int>(getResolvedPath(file_index).size());
        path_column_width = std::max(path_column_width, path_length + 2);
      }
    }
  }

  bool first_group = true;
  size_t group_number = 1;
  for (const auto& group : groups) {
    if (!first_group) {
      std::cout << "\n";
    }
    first_group = false;

    if (options.verbose) {
      std::cout << "Group " << group_number++ << " (" << group.size() << " files, "
                 << files[group.front()].getSize() << " bytes each):\n";
      PrintTableRow("Path", "Size (bytes)", path_column_width);
      std::cout << std::string(path_column_width + kValueColumnWidth, '-') << "\n";
      for (size_t file_index : group) {
        PrintTableRow(getResolvedPath(file_index), std::to_string(files[file_index].getSize()),
                      path_column_width);
      }
    } else {
      for (size_t file_index : group) {
        std::cout << "  " << getResolvedPath(file_index) << "\n";
      }
    }
  }
}

}  // namespace bayan
