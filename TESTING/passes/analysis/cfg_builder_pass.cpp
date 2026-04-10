#include "cfg_builder_pass.h"
#include <unordered_map>

CFG CFGBuilderPass::run(
    const std::vector<BasicBlock>& blocks)
{
    CFG cfg;
    cfg.blocks = blocks;

    std::unordered_map<std::string, int> labelMap;

    // Map labels → block index
    for(int i = 0; i < blocks.size(); i++)
    {
        for(auto &ins : blocks[i].instructions)
        {
            if(ins.type == LABEL)
                labelMap[ins.label] = i;
        }
    }

    for(int i = 0; i < blocks.size(); i++)
    {
        auto &block = blocks[i];
        auto &last = block.instructions.back();

        // GOTO
        if(last.type == GOTO)
        {
            cfg.edges[i].push_back(labelMap[last.label]);
        }

        // IF GOTO
        else if(last.type == IF_GOTO)
        {
            cfg.edges[i].push_back(labelMap[last.label]);

            if(i + 1 < blocks.size())
                cfg.edges[i].push_back(i + 1);
        }

        // NORMAL FLOW
        else
        {
            if(i + 1 < blocks.size())
                cfg.edges[i].push_back(i + 1);
        }
    }

    return cfg;
}