#pragma once
#include <vector>
#include <string>

struct Architecture {
    std::string name;

    std::vector<std::string> fastRegs;
    std::vector<std::string> slowRegs;
};