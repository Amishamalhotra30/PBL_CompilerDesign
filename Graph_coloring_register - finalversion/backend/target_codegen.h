#pragma once
#include "../core/instruction.h"
#include <unordered_map>
#include <vector>
#include <string>

class TargetCodegen {
public:

    static void generate(
        std::vector<Instruction>& program,
        std::unordered_map<std::string,std::string>& allocation);
};