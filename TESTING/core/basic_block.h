#pragma once
#include <vector>
#include <set>
#include "instruction.h"

struct BasicBlock {
    int id;
    std::vector<Instruction> instructions;

    std::set<std::string> USE;
    std::set<std::string> DEF;

    std::set<std::string> IN;
    std::set<std::string> OUT;
};