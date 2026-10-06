#include "solver.hpp"

template <typename Element, typename Product>
static Element power(Element base, int64_t e, Element identity, Product product)
{
    if (e == 0)
        return identity;

    if (e % 2 == 0)
    {
        const Element half = power(base, e / 2, identity, product);
        return product(half, half);
    }

    return product(base, power(base, e - 1, identity, product));
}

static int64_t multiplyModular(int64_t x, int64_t y)
{
    return x * y % MODULUS_CONST;
}

Affine compose(const Affine& outer, const Affine& inner)
{
    return {multiplyModular(outer.a, inner.a), (outer.a * inner.b + outer.b) % MODULUS_CONST};
}

Affine affinePower(const Affine& m, int64_t e)
{
    const auto product = [](const Affine& x, const Affine& y)
    { 
        return compose(x, y);
    };

    return power(m, e, Affine{1, 0}, product);
}

int64_t modularPower(int64_t base, int64_t e)
{
    const auto product = [](int64_t x, int64_t y) { return multiplyModular(x, y); };
    return power(base, e, int64_t{1}, product);
}

ModularFraction solve(const std::vector<int64_t>& a)
{
    const Affine T{MODULUS_CONST - INVERSE_OF_TWO, INVERSE_OF_TWO};  // x -> -x/2 + 1/2

    Affine Tn = T;
    int64_t two_to_n = 2;  // 2^n, built the same way as T^n
    for (const int64_t ai : a)
    {
        Tn = affinePower(Tn, ai);
        two_to_n = modularPower(two_to_n, ai);
    }

    const int64_t f_n = (Tn.a + Tn.b) % MODULUS_CONST;  // T^n(1)
    const int64_t q = multiplyModular(two_to_n, INVERSE_OF_TWO);  // 2^(n-1)

    return {multiplyModular(f_n, q), q};
}
