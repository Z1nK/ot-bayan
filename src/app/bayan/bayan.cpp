#include "bayan.hpp"

#include <bayan/cli-parser/cli-parser.hpp>
#include <bayan/filesystem-helper/file_finder.hpp>

#include <bayan/version/version.hpp>

#include <iostream>

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

  return options;
}

void Bayan::printOptions(const BayanOptions& options) const {
  std::cout << "Bayan - File Duplicate Finder" << std::endl;
  std::cout << "==============================" << std::endl;

  std::cout << "Scan directories: ";
  for (const auto& dir : options.scan_dirs) {
    std::cout << dir.string() << " ";
  }
  std::cout << std::endl;

  if (!options.exclude_dirs.empty()) {
    std::cout << "Exclude directories: ";
    for (const auto& dir : options.exclude_dirs) {
      std::cout << dir.string() << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "Scan depth: " << options.depth << std::endl;
  std::cout << "Minimum file size: " << options.min_size << " bytes" << std::endl;

  if (!options.masks.empty()) {
    std::cout << "File masks: ";
    for (const auto& mask : options.masks) {
      std::cout << mask << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "Block size: " << options.block_size << std::endl;
  std::cout << "Hash algorithm: " << options.hash_algorithm << std::endl;
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

  printOptions(options);

  // TODO: Implement actual duplicate finding logic here
  FileFinder finder;
  finder.AddScanDir(options.scan_dirs)
        .AddExcludeDir(options.exclude_dirs)
        .SetScanDepth(options.depth)
        .SetMinFileSize(options.min_size)
        .AddMask(options.masks);

  std::vector<fs::path> files = finder.Find();  // This will return the list of files, but we are not using it yet.

  for (const auto& file : files) {
    std::cout << "Found file: " << file.string() << std::endl;
  }
}

}  // namespace bayan
