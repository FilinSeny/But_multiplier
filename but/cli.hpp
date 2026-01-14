#include "but/config.hpp"
#include <string>

namespace but {

std::string usage(std::string_view exe);
Config parse_args(int argc, char** argv); // throws std::runtime_error

}