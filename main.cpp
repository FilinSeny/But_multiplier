#include "but/cli.cpp"
#include "but/generator.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char **argv)
{
    try
    {

        but::Config config = but::parse_args(argc, argv);

        but::Generator gen;
        gen.validate(config);

        std::string verilog = gen.generate_verilog(config);

        std::ofstream out(config.out_path, std::ios::binary);
        if (!out)
            throw std::runtime_error("cannot open output file");

        out << verilog;
        if (!out)
            throw std::runtime_error("write failed");

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }
}
