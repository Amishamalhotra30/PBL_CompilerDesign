#include "graph_coloring_pass.h"
#include <algorithm>

void GraphColoringPass::run(
    std::unordered_map<std::string,std::set<std::string>>& graph,
    std::vector<std::string> registers,
    std::unordered_map<std::string,int>& usage)
{
    std::vector<std::string> nodes;

    for(auto &n : graph)
        nodes.push_back(n.first);

    // Sort variables by frequency (highest first)
    std::sort(nodes.begin(), nodes.end(),
        [&](const std::string &a, const std::string &b)
    {
        return usage[a] > usage[b];
    });

    for(auto &var : nodes)
    {
        std::set<std::string> used;

        for(auto &nbr : graph[var])
        {
            if(allocation.count(nbr))
                used.insert(allocation[nbr]);
        }

        bool assigned=false;

        for(auto &reg : registers)
        {
            if(used.find(reg)==used.end())
            {
                allocation[var]=reg;
                assigned=true;
                break;
            }
        }

        if(!assigned)
            allocation[var]="SPILL";
    }
}
