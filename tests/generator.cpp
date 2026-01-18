#include<fstream>
#include<iostream>
#include<sstream>
#include<random>
#include"but/config.hpp"
#include<cmath>


namespace but {
    struct TestValues {
        int m, r;
        long long res;

        TestValues(int _m, int _r) : m(_m), r(_r) {
            res = m * r;
        }
    };


    TestValues gen_one_test(std::mt19937 & rng,
                            std::uniform_int_distribution<int64_t> & dist_m, 
                            std::uniform_int_distribution<int64_t> & dist_r,
                             std::string args_signs) {
        int m = (args_signs[0] == '-' ? -1 : 1) * dist_m(rng);
        int r = (args_signs[1] == '-' ? -1 : 1) * dist_r(rng);

        return(TestValues(m, r));
    }


    static void gen_testbanch(const Config& conf, int seed = 42) {
        std::mt19937 rng(seed);
        int lo_m = 0;
        int hi_m =  (1ll << (conf.m_size - 1)) - 1;
        std::uniform_int_distribution<int64_t> dist_m(lo_m, hi_m);
        int lo_r = 0;
        int hi_r =  (1ll << (conf.r_size - 1)) - 1;
        std::uniform_int_distribution<int64_t> dist_r(lo_r, hi_r);
        const int number_of_tests = std::min(std::max((1ll << conf.res_size), 100), 1e8);
        
        
        std::ofstream o(config.out_path, std::ios::binary);
        if (!out)
            throw std::runtime_error("cannot open output file");

        out << verilog;
    }      

        o << "module testbanch#(\n"
        << "  parameter m_size   = " << conf.m_size << ",\n"
        << "  parameter r_size   = " << conf.r_size << ",\n"
        << "  parameter res_size = m_size + r_size\n"
        << ")();\n\n"
        << "  reg signed [m_size-1:0]    M;\n"
        << "  reg signed [r_size-1:0]    R;\n"
        << "  wire signed [res_size-1:0] RES;\n\n"
        << "  But_multiplier exmp(\n"
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

        

        ///ручные тесты для сложных ситуаций

        ///динамически изменяемое число unit-тестов
        std::string test_mask[4] = {"++", "--", "+-", "-+"};

        for (int i = 0; i < number_of_tests / 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                TestValues tv = gen_one_test(rng, dist_m, dist_r, test_mask[j]);
                o << "        test(" << tv.m << ", " << tv.r << ", " << tv.res << ")\n" 
                << "      #1;";
            }
        }

         o << "    $display(\"PASS %s\", `__FILE__);\n"
        << "    $finish;\n"
        << "  end\n\n"
        << "endmodule\n";

        

}

