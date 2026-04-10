#include "interference_graph_pass.h"
#include <cctype>

void InterferenceGraphPass::run(CFG &cfg)
{
    graph.clear();

    for (auto &block : cfg.blocks)
    {
        std::set<std::string> live = block.OUT;

        for (auto it = block.instructions.rbegin();
             it != block.instructions.rend(); ++it)
        {
            auto &ins = *it;

            // STEP 1: Add LIVE ↔ LIVE interference
            for (auto it1 = live.begin(); it1 != live.end(); ++it1)
            {
                for (auto it2 = std::next(it1); it2 != live.end(); ++it2)
                {
                    graph[*it1].insert(*it2);
                    graph[*it2].insert(*it1);
                }
            }

            std::string dest = ins.dest;

            if (!dest.empty())
            {
                // Ensure node exists
                graph[dest];

                //  STEP 2: dest ↔ LIVE interference
                for (auto &v : live)
                {
                    if (v.empty()) continue;

                    graph[v]; // ensure node exists

                    if (v != dest)
                    {
                        graph[dest].insert(v);
                        graph[v].insert(dest);
                    }
                }

                // Remove dest from LIVE
                live.erase(dest);
            }

            // STEP 3: Add sources to LIVE
            if (!ins.src1.empty() && std::isalpha(ins.src1[0]))
                live.insert(ins.src1);

            if (!ins.src2.empty() && std::isalpha(ins.src2[0]))
                live.insert(ins.src2);
        }
    }
}