#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "oracle.hpp"
#include "solver.hpp"

using Input = std::vector<int64_t>;

class TestRunner {
public:
    void expectEqual(const Input& a, const ModularFraction& expected,
                     const std::string& description)
    {
        const ModularFraction actual = solve(a);
        ++total_;
        if (!(actual == expected))
        {
            ++failures_;
            std::cerr << "FAILED: " << description << " -> expected " << expected
                      << ", got " << actual << '\n';
        }
    }

    int finish(const std::string& name) const
    {
        std::cout << name << ": " << (total_ - failures_) << "/" << total_
                  << " cases passed\n";
        return failures_ == 0 ? 0 : 1;
    }

private:
    int total_ = 0;
    int failures_ = 0;
};

static ModularFraction closedFormReference(const Input& a)
{
    constexpr int64_t INVERSE_OF_THREE = (MODULUS_CONST + 1) / 3;

    int64_t n_mod_fermat = 1;
    bool n_is_even = false;
    for (const int64_t ai : a)
    {
        n_mod_fermat = n_mod_fermat * (ai % (MODULUS_CONST - 1)) % (MODULUS_CONST - 1);
        n_is_even = n_is_even || ai % 2 == 0;
    }

    int64_t exponent = (n_mod_fermat + MODULUS_CONST - 2) % (MODULUS_CONST - 1);  // n-1
    int64_t q = modularPower(2, exponent);
    int64_t sign = n_is_even ? 1 : MODULUS_CONST - 1;  // (-1)^n
    return {(q + sign) % MODULUS_CONST * INVERSE_OF_THREE % MODULUS_CONST, q};
}

static int runUnitTests()
{
    TestRunner tests;

    tests.expectEqual({2}, {1, 2}, "Codeforces example 1");
    tests.expectEqual({1, 1, 1}, {0, 1}, "Codeforces example 2");

    ExactRecurrence oracle;
    for (int n = 1; n <= ORACLE_MAX_N; ++n)
    {
        const ModularFraction expected = toModular(oracle.f(n));
        for (int d = 1; d <= n; ++d)
        {
            if (n % d != 0)
                continue;

            tests.expectEqual({d, n / d}, expected,
                              "n = " + std::to_string(d) + " * " + std::to_string(n / d));
        }
    }

    tests.expectEqual({1}, {0, 1}, "n = 1");
    tests.expectEqual(Input(100000, 1), {0, 1}, "k = 10^5, all ones");
    tests.expectEqual({MODULUS_CONST - 1}, {INVERSE_OF_TWO, INVERSE_OF_TWO},
                      "n = MOD-1: 2^(n-1) = 2^(MOD-2) = 1/2");
    tests.expectEqual({MODULUS_CONST}, {0, 1}, "n = MOD: p is a multiple of MOD");

    return tests.finish("unit");
}

static int runStressTests()
{
    TestRunner tests;
    std::mt19937_64 generator(2026);

    const std::vector<int64_t> special_values =
    {
        1, 2, 3, MODULUS_CONST - 1, MODULUS_CONST, MODULUS_CONST + 1, int64_t{1} << 59,
        1'000'000'000'000'000'000, 999'999'999'999'999'999
    };

    for (int test = 0; test < 3000; ++test)
    {
        Input a(1 + generator() % 50);
        for (int64_t& ai : a)
        {
            ai = (generator() % 4 == 0)
                     ? special_values[generator() % special_values.size()]
                     : static_cast<int64_t>(1 + generator() % 1'000'000'000'000'000'000);
        }
        tests.expectEqual(a, closedFormReference(a), "random case " + std::to_string(test));
    }

    const Input largest(100000, 1'000'000'000'000'000'000);
    tests.expectEqual(largest, closedFormReference(largest), "k = 10^5, all 10^18");

    return tests.finish("stress");
}

int main(int argc, char** argv)
{
    const std::string mode = (argc > 1) ? argv[1] : "";
    if (mode == "unit") return runUnitTests();
    if (mode == "stress") return runStressTests();
    std::cerr << "Usage: " << argv[0] << " unit|stress\n";
    return 2;
}
