#include "cli-parser.hpp"
#include <iostream>
#include <bayan/version/version.hpp>

namespace bayan {

CliParser::CliParser() : options_desc("Allowed options") {
  options_desc.add_options()
  ("help,h", "Produce help message")
  ("scan,s", po::value<std::vector<std::string>>()->multitoken(),"Directories to scan")
  ("exclude,e", po::value<std::vector<std::string>>()->multitoken(), "Directories to exclude")
  ("depth,d", po::value<size_t>()->default_value(0), "Scan depth (0 - current dir only)")
  ("min-size,m", po::value<uint64_t>()->default_value(1), "Minimum file size in bytes")
  ("mask", po::value<std::vector<std::string>>()->multitoken(), "Filename masks (case-insensitive)")
  ("block-size,b", po::value<size_t>()->default_value(5), "Block size S")
  ("hash", po::value<std::string>()->default_value("crc32"),"Hash algorithm (crc32, md5)")
  ("version,v", "Show version information");
}

po::variables_map CliParser::parse(int argc, char *argv[]) {
  po::variables_map vm;

  try {
    po::store(po::parse_command_line(argc, argv, options_desc), vm);
    po::notify(vm);
  } catch (const std::exception &e) {
    std::cerr << "Error parsing command line arguments: " << e.what()
              << std::endl;
    throw;
  }

  return vm;
}

void CliParser::printHelp() const { std::cout << options_desc << std::endl; }

} // namespace bayan