
#include "but/config.hpp"
#include <string>

namespace but {

class Generator {
public:
  void validate(const Config& cfg) const;         
  std::string generate_verilog(const Config& cfg) const;

};

} // namespace booth
