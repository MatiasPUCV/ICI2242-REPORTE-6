#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "solver.hpp"

struct ExactFraction
{
    int64_t num;
    int64_t den;
};


inline constexpr int ORACLE_MAX_N = 60;

ModularFraction toModular(const ExactFraction& fraction);


class ExactRecurrence
{
public:
    
    ExactRecurrence();
    ExactFraction f(int n);
    ExactFraction g(int n);

private:

    std::vector<std::optional<ExactFraction>> memo_f_;
    std::vector<std::optional<ExactFraction>> memo_g_;
};
