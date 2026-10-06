#include "oracle.hpp"

#include <numeric>
#include <stdexcept>

static ExactFraction reduce(int64_t num, int64_t den)
{
    const int64_t d = std::gcd(num, den);
    return {num / d, den / d};
}

static ExactFraction add(const ExactFraction& x, const ExactFraction& y)
{
    const int64_t den = x.den / std::gcd(x.den, y.den) * y.den;
    return reduce(x.num * (den / x.den) + y.num * (den / y.den), den);
}

static ExactFraction half(const ExactFraction& x)
{ 
    return reduce(x.num, x.den * 2);
}

static void checkRange(int n)
{
    if (n < 0 || n > ORACLE_MAX_N)
    {
        throw std::out_of_range("n is outside the range supported by the oracle");
    }
}

ModularFraction toModular(const ExactFraction& fraction)
{
    return {fraction.num % MODULUS_CONST, fraction.den % MODULUS_CONST};
}

ExactRecurrence::ExactRecurrence()
    : memo_f_(ORACLE_MAX_N + 1), memo_g_(ORACLE_MAX_N + 1)
{

}

ExactFraction ExactRecurrence::f(int n)
{
    checkRange(n);
    if (n == 0) return {1, 1};
    if (!memo_f_[n]) memo_f_[n] = g(n - 1);
    return *memo_f_[n];
}

ExactFraction ExactRecurrence::g(int n)
{
    checkRange(n);
    if (n == 0) return {0, 1};
    if (!memo_g_[n]) memo_g_[n] = half(add(f(n - 1), g(n - 1)));
    return *memo_g_[n];
}
