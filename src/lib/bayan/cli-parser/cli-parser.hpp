#pragma once
#include <boost/program_options.hpp>
#include <vector>
#include <string>
#include <cstdint>

namespace po = boost::program_options;

namespace bayan {

class CliParser {
public:
    CliParser();
    po::variables_map parse(int argc, char* argv[]);
    void printHelp() const;

private:
    po::options_description options_desc;
};

} // namespace bayan
