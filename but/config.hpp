#include <string>

namespace but {



struct Config {
  unsigned int m_size = 4;
  unsigned int r_size = 4;
  unsigned int res_size;

  std::string module_name = "But_multiplier";
  std::string out_path;
};

}
