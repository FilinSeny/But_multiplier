#include "but/generator.hpp"
#include <sstream>
#include <stdexcept>

namespace but {

void Generator::validate(const Config& conf) const {
  if (conf.m_size <= 0 || conf.r_size <= 0)
    throw std::runtime_error("widths must be positive");
}

std::string Generator::generate_verilog(const Config& conf) const {
  const int m   = conf.m_size;
  const int r   = conf.r_size;
  const int res = conf.res_size;

  std::ostringstream o;

  o << "module  But_multiplier" << m << "x" << r << "#(\n"
  << "    parameter m_size =      " << m << ", ///m_size always greater or equal to r_size\n"
  << "    parameter r_size =      " << r << ",\n\n"
  << "    parameter res_size =    " << res << "\n"
  << ")\n"
  << "(\n"
  << "    input wire[m_size - 1:0]    M,\n"
  << "    input wire[r_size - 1:0]    R,\n"
  << "    output wire [res_size - 1: 0]   RES\n"
  << ");\n\n\n"
  << "    wire        [r_size:0] zero;\n"
  << "    assign      zero    = 0;\n"
  << "    wire signed [m_size:0] M_s;\n"
  << "    assign M_s = {M[m_size - 1], $signed(M)};\n\n"
  << "    wire signed [m_size:0] M_neg;\n"
  << "    assign M_neg = -M_s;\n"
  << "    \n"
  << "    wire signed [res_size + 1:0] A = {M_s, zero};\n"
  << "    wire signed [res_size + 1:0] S = {M_neg, zero};\n"
  << "    \n"
  << "    wire signed [res_size + 1:0] P = {{(m_size + 1){1'b0}}, R, 1'b0};\n"
  << "     \n"
  << "    wire signed [res_size + 1:0] Pi_res [0:r_size];\n\n"
  << "    assign Pi_res[0] = P;\n"
  << "    \n"
  << "    genvar i;\n"
  << "    generate\n"
  << "        for (i = 0; i < r_size; i = i + 1) begin : but_stages\n"
  << "            wire signed [res_size + 1:0] P_before_shift;\n"
  << "            \n"
  << "            assign P_before_shift = \n"
  << "                (Pi_res[i][1:0] == 2'b01) ? Pi_res[i] + A :\n"
  << "                (Pi_res[i][1:0] == 2'b10) ? Pi_res[i] + S :\n"
  << "                Pi_res[i];\n"
  << "            \n"
  << "            assign Pi_res[i+1] = P_before_shift >>> 1;\n"
  << "        end\n"
  << "    endgenerate\n"
  << "    \n"
  << "    assign RES = Pi_res[r_size][res_size:1];\n\n\n"
  << "endmodule\n";


  return o.str();
}

}
