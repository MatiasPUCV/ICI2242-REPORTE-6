#pragma once

#include <cstdint>
#include <ostream>
#include <vector>

inline constexpr int64_t MODULUS_CONST = 1'000'000'007;
inline constexpr int64_t INVERSE_OF_TWO = (MODULUS_CONST + 1) / 2;

struct ModularFraction
{
    int64_t p;
    int64_t q;
};

struct Affine
{
    int64_t a;
    int64_t b;
};

Affine compose(const Affine& outer, const Affine& inner);
Affine affinePower(const Affine& m, int64_t e);
int64_t modularPower(int64_t base, int64_t e);
ModularFraction solve(const std::vector<int64_t>& a);

inline bool operator==(const ModularFraction& x, const ModularFraction& y)
{
    return x.p == y.p && x.q == y.q;
}

inline std::ostream& operator<<(std::ostream& out, const ModularFraction& fraction)
{
    return out << fraction.p << '/' << fraction.q;
}
