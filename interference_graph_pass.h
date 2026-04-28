#pragma once
#include <unordered_map>
#include <set>
#include <string>
#include "../../core/cfg.h"

class InterferenceGraphPass {
public:
    std::unordered_map<std::string, std::set<std::string>> graph;

    void run(CFG &cfg);
};