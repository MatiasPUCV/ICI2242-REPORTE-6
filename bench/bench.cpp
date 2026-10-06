#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

#include "solver.hpp"

static constexpr int BITS = 60;
static constexpr int REPETITIONS = 7;

static std::vector<int64_t> generateInput(std::mt19937_64& generator, int k)
{
    const uint64_t high_bit = uint64_t{1} << (BITS - 1);
    std::vector<int64_t> a(k);
    for (int64_t& ai : a)
    {
        ai = static_cast<int64_t>(high_bit | (generator() & (high_bit - 1)));
    }

    return a;
}

static double measureMs(const std::vector<int64_t>& a, int64_t& sink)
{
    std::vector<double> times;
    for (int i = 0; i < REPETITIONS; ++i)
    {
        const auto start = std::chrono::steady_clock::now();
        const ModularFraction result = solve(a);
        const auto end = std::chrono::steady_clock::now();
        sink ^= result.p ^ result.q;
        times.push_back(std::chrono::duration<double, std::milli>(end - start).count());
    }
    std::sort(times.begin(), times.end());
    return times[times.size() / 2];
}

int main()
{
    std::mt19937_64 generator(12345);
    int64_t sink = 0;

    std::cout << "k,time_ms\n";
    for (const int k : {1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000, 1000000})
    {
        const std::vector<int64_t> a = generateInput(generator, k);
        std::cout << k << ',' << measureMs(a, sink) << '\n';
    }

    std::cerr << "(sink " << sink << ")\n";
    return 0;
}
