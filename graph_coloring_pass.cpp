#include "graph_coloring_pass.h"
#include <algorithm>
#include <iostream>

void GraphColoringPass::run(
    std::unordered_map<std::string,std::set<std::string>>& graph,
    std::vector<std::string> registers,
    std::unordered_map<std::string,int>& usage)
{
    allocation.clear();
    spillCost.clear();   // ✅ IMPORTANT

    std::vector<std::string> nodes;

    for(auto &n : graph)
        nodes.push_back(n.first);

    // Sort HOT variables first
    std::sort(nodes.begin(), nodes.end(),
        [&](const std::string &a, const std::string &b)
    {
        return usage[a] > usage[b];
    });

    // Spill cost function
    auto computeSpillCost = [&](const std::string &var) {
        int degree = graph[var].size();
        int use = usage[var];

        if(degree == 0) degree = 1;

        return (double)use / degree;
    };

    for(auto &var : nodes)
    {
        std::set<std::string> used;

        for(auto &nbr : graph[var])
        {
            if(allocation.count(nbr) && allocation[nbr] != "SPILL")
                used.insert(allocation[nbr]);
        }

        bool assigned = false;

        for(auto &reg : registers)
        {
            if(used.find(reg) == used.end())
            {
                allocation[var] = reg;
                assigned = true;
                break;
            }
        }

        // 🔥 SPILL CASE
        if(!assigned)
        {
            double cost = computeSpillCost(var);

            allocation[var] = "SPILL";

            spillCost[var] = cost;   // ✅ THIS WAS MISSING

            // UI-friendly format
            std::cout << "[SPILL] " << var
                      << " cost=" << cost << std::endl;
        }
    }
}