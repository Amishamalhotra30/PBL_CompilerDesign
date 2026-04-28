#pragma once
#include <vector>
#include <string>
#include "../core/instruction.h"
#include <unordered_map>
class TACParser {
public:
    std::vector<Instruction> parse(const std::string &filename);
    std::unordered_map<std::string, int> usageCount;
};