#include "bayan.hpp"

#include <bayan/cli-parser/cli-parser.hpp>
#include <bayan/filesystem-helper/file_finder.hpp>
#include <bayan/filesystem-helper/file_obj.hpp>
#include <bayan/hash/hash_factory.hpp>

#include <bayan/version/version.hpp>

#include <iostream>
#include <format>

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

  // printOptions(options);

  
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

  printDuplicateGroups(files, duplicate_groups);
}

void Bayan::printDuplicateGroups(const std::vector<FileObj>& files,
                                  const std::vector<DuplicateFinder::Group>& groups) const {
  if (groups.empty()) {
    // std::cout << "No duplicates found." << std::endl;
    return;
  }

  // std::cout << std::format("Found {} group(s) of duplicates:", groups.size()) << std::endl;

  // size_t group_number = 1;
  bool first_group = true;
  for (const auto& group : groups) {
    if (!first_group) {
      std::cout << "\n";
    }
    first_group = false;

    // std::cout << std::format("Group {} ({} files, {} bytes each):", group_number++, group.size(),
    //                           files[group.front()].getSize())
    //            << std::endl;
    for (size_t file_index : group) {
      std::cout << "  " << files[file_index].getPath().string() << "\n";
    }
  }
}

}  // namespace bayan
