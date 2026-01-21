#include "but/cli.hpp"
#include <charconv>
#include <sstream>
#include <stdexcept>
#include <string_view>

namespace but
{

    static int parse_positive_int(std::string_view s, std::string_view what)
    {

        int v = 0;
        auto [ptr, err] = std::from_chars(s.data(), s.data() + s.size(), v);

        if (err != std::errc{} || ptr != s.data() + s.size())
            throw std::runtime_error(std::string(what) + ": not an integer");
        if (v <= 0)
            throw std::runtime_error(std::string(what) + ": must be > 0");
        return v;
    }

    Config parse_args(int argc, char **argv)
    {

        if (argc <= 1)
            throw std::runtime_error("too few arguments");

        Config conf;
        ///bool   need_testfile = false;

        for (int i = 1; i < argc; ++i)
        {
            std::string_view opt = argv[i];

            /// go to next after param name
            auto require = [&](std::string_view name) -> std::string_view
            {
                if (++i >= argc)
                    throw std::runtime_error("missing value for " + std::string(name));
                return argv[i];
            };

            if (opt == "--help" || opt == "-h")
            {
                throw std::runtime_error(
                    "Use --m for first val size\n"
                    "Use --r for second val size\n"
                    "Use --out to define out.v\n"
                    "Use --test to generate testbanch, second val must be seed\n");
            }
            else if (opt == "--m")
            {
                conf.m_size = parse_positive_int(require(opt), "--m");
            }
            else if (opt == "--r")
            {
                conf.r_size = parse_positive_int(require(opt), "--r");
            }
            else if (opt == "--out")
            {
                conf.out_path = std::string(require(opt));
            }
            else if (opt == "--test") {
                conf.need_testfile = true;
                conf.test_seed = parse_positive_int(require(opt), "--seed");
            }
            else
            {
                throw std::runtime_error("unknown option: " + std::string(opt));
            }
        }

        if (conf.out_path.empty()) {
            std::ostringstream oss;
            oss << "But_multiplier"
                << conf.m_size
                << "x"
                << conf.r_size;

            conf.module_name = oss.str();
            conf.out_path = conf.module_name + ".v";
            ///throw std::runtime_error("missing required option --out");

        }
        conf.res_size = conf.m_size + conf.r_size;



        return conf;
    }

}