#pragma once
#include "../../core/basic_block.h"

class BasicBlockPass {
public:
    std::vector<BasicBlock> run(
        const std::vector<Instruction>& program);
};