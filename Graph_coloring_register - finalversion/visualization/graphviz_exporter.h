#pragma once
#include <unordered_map>
#include <set>
#include <string>

class GraphvizExporter {
public:
    static void exportInterferenceGraph(
        const std::unordered_map<std::string, std::set<std::string>>& graph,
        const std::unordered_map<std::string, std::string>& allocation,
        const std::string& filename
    );
};