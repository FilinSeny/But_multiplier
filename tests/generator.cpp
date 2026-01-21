#include <fstream>
#include <iostream>
#include <sstream>
#include <random>
#include "but/config.hpp"
#include <cmath>
#include <algorithm>
#include <climits>
#include <limits>
#include <stdexcept>

namespace but {

struct TestValues {
    int m, r;
    long long res;

    TestValues(int _m, int _r) : m(_m), r(_r), res(1LL * _m * _r) {}
};

static TestValues gen_one_test(std::mt19937 &rng,
                               std::uniform_int_distribution<int64_t> &dist_m,
                               std::uniform_int_distribution<int64_t> &dist_r,
                               const std::string &args_signs) {
    int64_t m_mag = dist_m(rng);
    int64_t r_mag = dist_r(rng);

    int64_t m = (args_signs.size() > 0 && args_signs[0] == '-') ? -m_mag : m_mag;
    int64_t r = (args_signs.size() > 1 && args_signs[1] == '-') ? -r_mag : r_mag;

   
    if (m < std::numeric_limits<int>::min() || m > std::numeric_limits<int>::max() ||
        r < std::numeric_limits<int>::min() || r > std::numeric_limits<int>::max()) {
        throw std::runtime_error("generated value doesn't fit into int");
    }

    return TestValues((int)m, (int)r);
}

static void gen_testbanch(const Config &conf) {
    const std::string test_path = conf.module_name + "_testbanch.v";

    try {
        std::mt19937 rng(conf.test_seed);

        if (conf.m_size < 2 || conf.r_size < 2) {
            throw std::runtime_error("m_size and r_size must be >= 2 for signed ranges");
        }

        const int64_t lo_m = 0;
        const int64_t hi_m = (conf.m_size >= 63) ? (std::numeric_limits<int64_t>::max)()
                                                 : ((1LL << (conf.m_size - 1)) - 1);
        std::uniform_int_distribution<int64_t> dist_m(lo_m, hi_m);

        const int64_t lo_r = 0;
        const int64_t hi_r = (conf.r_size >= 63) ? (std::numeric_limits<int64_t>::max)()
                                                 : ((1LL << (conf.r_size - 1)) - 1);
        std::uniform_int_distribution<int64_t> dist_r(lo_r, hi_r);

        long long base;
        if (conf.res_size >= 62) 
            base = std::numeric_limits<long long>::max();
        else 
            base = 1LL << conf.res_size;

        long long nll = std::min(std::max(base, 100LL), 100000000LL);
        int number_of_tests = (nll > INT_MAX) ? INT_MAX : (int)nll;

        std::ofstream testfile_out(test_path, std::ios::binary);
        if (!testfile_out) {
            throw std::runtime_error("cannot open output file: " + test_path);
        }

        testfile_out.exceptions(std::ios::badbit | std::ios::failbit);

        testfile_out
            << "module testbanch#(\n"
            << "  parameter m_size   = " << conf.m_size << ",\n"
            << "  parameter r_size   = " << conf.r_size << ",\n"
            << "  parameter res_size = m_size + r_size\n"
            << ")();\n\n"
            << "  reg  signed [m_size-1:0]    M;\n"
            << "  reg  signed [r_size-1:0]    R;\n"
            << "  wire signed [res_size-1:0]  RES;\n\n"
            << "  " << conf.module_name << " exmp(\n"
            << "    .M(M),\n"
            << "    .R(R),\n"
            << "    .RES(RES)\n"
            << "  );\n\n"
            << "  task test;\n"
            << "    input signed [m_size-1:0]   t_a;\n"
            << "    input signed [r_size-1:0]   t_b;\n"
            << "    input signed [res_size-1:0] t_mul;\n"
            << "    begin\n"
            << "      M = t_a;\n"
            << "      R = t_b;\n"
            << "      #1;\n"
            << "      $display(\"TEST %0d * %0d = %0d\", t_a, t_b, RES);\n"
            << "      if (RES !== t_mul) begin\n"
            << "        $display(\"FAIL %s: expected %0d got %0d\", `__FILE__, t_mul, RES);\n"
            << "        $finish;\n"
            << "      end\n"
            << "    end\n"
            << "  endtask\n\n"
            << "  initial begin\n"
            << "    $dumpfile(\"dump.vcd\");\n"
            << "    $dumpvars(0, testbanch);\n"
            << "    #1;\n";

        const std::string test_mask[4] = {"++", "--", "+-", "-+"};
        int blocks = number_of_tests / 4;

        for (int i = 0; i < blocks; ++i) {
            for (int j = 0; j < 4; ++j) {
                TestValues tv = gen_one_test(rng, dist_m, dist_r, test_mask[j]);

                testfile_out
                    << "    test(" << tv.m << ", " << tv.r << ", " << tv.res << ");\n"
                    << "    #1;\n";
            }
        }

        testfile_out
            << "    $display(\"PASS %s\", `__FILE__);\n"
            << "    $finish;\n"
            << "  end\n\n"
            << "endmodule\n";

        testfile_out.flush();

    } catch (const std::ios_base::failure &e) {
        throw std::runtime_error(
            "I/O error while generating testbench '" + test_path + "': " + e.what()
        );
    } catch (const std::exception &e) {
        throw std::runtime_error(
            "gen_testbanch failed for module '" + conf.module_name +
            "' (path: " + test_path + "): " + e.what()
        );
    }
}

}
