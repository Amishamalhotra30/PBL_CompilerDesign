#include "peephole_pass.h"
#include <set>

void PeepholePass::run(std::vector<Instruction>& program)
{
    std::set<std::string> used;

    // Track variables used in expressions ONLY
    for(auto &ins : program)
    {
        if(ins.type == BINARY)
        {
            used.insert(ins.src1);
            used.insert(ins.src2);
        }
    }

    std::vector<Instruction> optimized;

    for(size_t i = 0; i < program.size(); i++)
    {
        auto &ins = program[i];

        // NEVER REMOVE CONTROL FLOW
        if(ins.type == LABEL ||
           ins.type == GOTO ||
           ins.type == IF_GOTO)
        {
            optimized.push_back(ins);
            continue;
        }

        // Keep if used
        if(used.find(ins.dest) != used.end())
        {
            optimized.push_back(ins);
        }
    }

    program = optimized;
}