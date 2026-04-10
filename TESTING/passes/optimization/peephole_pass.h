#pragma once
#include "../../core/instruction.h"
#include <vector>

class PeepholePass {
public:

    static void run(std::vector<Instruction>& program);
};