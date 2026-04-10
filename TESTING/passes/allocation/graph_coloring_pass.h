#pragma once
#include <unordered_map>
#include <set>
#include <string>
#include <vector>

class GraphColoringPass {
public:
    std::unordered_map<std::string, std::string> allocation;

    void run(
        std::unordered_map<std::string,std::set<std::string>>& graph,
        std::vector<std::string> registers,
        std::unordered_map<std::string,int>& usage
    );
};