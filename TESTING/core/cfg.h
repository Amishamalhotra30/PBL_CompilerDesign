#pragma once
#include <vector>
#include <unordered_map>
#include "basic_block.h"

struct CFG {
    std::vector<BasicBlock> blocks;

    // adjacency list
    std::unordered_map<int, std::vector<int>> edges;
};