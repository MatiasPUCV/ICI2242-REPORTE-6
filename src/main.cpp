#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "solver.hpp"

static constexpr int64_t MAX_ELEMENT = 1'000'000'000'000'000'000;  // 10^18

static std::vector<int64_t> readInput(std::istream& in)
{
    size_t k = 0;
    if (!(in >> k) || k == 0) {
        throw std::runtime_error("expected k >= 1 on the first line");
    }

    std::vector<int64_t> a(k);
    for (int64_t& ai : a)
    {
        if (!(in >> ai) || ai < 1 || ai > MAX_ELEMENT)
        {
            throw std::runtime_error("every a_i must be between 1 and 10^18");
        }
    }

    return a;
}

static std::vector<int64_t> readInputFile(const char* path)
{
    std::ifstream file(path);
    if (!file) throw std::runtime_error(std::string("cannot open ") + path);
    return readInput(file);
}

int main(int argc, char** argv)
{
    try
    {
        const std::vector<int64_t> a = (argc > 1) ? readInputFile(argv[1]) : readInput(std::cin);
        std::cout << solve(a) << '\n';
    }
    catch (const std::exception& error)
    {
        std::cerr << "error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
