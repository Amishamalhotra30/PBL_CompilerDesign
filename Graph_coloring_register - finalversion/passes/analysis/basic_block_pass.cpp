#include "basic_block_pass.h"
#include <set>

std::vector<BasicBlock> BasicBlockPass::run(
    const std::vector<Instruction>& program)
{
    std::vector<BasicBlock> blocks;
    BasicBlock current;
    current.id = 0;

    for(int i = 0; i < program.size(); i++)
    {
        auto &ins = program[i];

        // Start new block at LABEL
        if(ins.type == LABEL && !current.instructions.empty())
        {
            blocks.push_back(current);
            current = BasicBlock();
            current.id = blocks.size();
        }

        current.instructions.push_back(ins);

        // End block on control flow
        if(ins.type == GOTO || ins.type == IF_GOTO)
        {
            blocks.push_back(current);
            current = BasicBlock();
            current.id = blocks.size();
        }
    }

    if(!current.instructions.empty())
        blocks.push_back(current);

    // FIX USE / DEF
    for(auto &block : blocks)
    {
        std::set<std::string> defined;

        for(auto &ins : block.instructions)
        {
            if(ins.type == BINARY)
            {
                if(!ins.src1.empty() && !defined.count(ins.src1))
                    block.USE.insert(ins.src1);

                if(!ins.src2.empty() && !defined.count(ins.src2))
                    block.USE.insert(ins.src2);

                if(!ins.dest.empty())
                {
                    block.DEF.insert(ins.dest);
                    defined.insert(ins.dest);
                }
            }
        }
    }

    return blocks;
}