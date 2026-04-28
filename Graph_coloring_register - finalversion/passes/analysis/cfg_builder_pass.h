#pragma once
#include "../../core/cfg.h"

class CFGBuilderPass {
public:
    CFG run(const std::vector<BasicBlock>& blocks);
};