#include "tac_parser.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<Instruction> TACParser::parse(const std::string &filename) {

    std::vector<Instruction> instructions;
    usageCount.clear(); // reset for new file

    std::ifstream file(filename);
if(!file.is_open())
{
    std::cout << "ERROR: Cannot open file -> " << filename << "\n";
    return {};
}
    
    std::string line;

    while (getline(file, line)) {

        if(line.empty()) continue;

        std::cout << "Reading line: " << line << "\n";

        Instruction ins;

        // =========================
        // LABEL (L1:)
        // =========================
        if(line.back() == ':') {
            ins.type = LABEL;
            ins.label = line.substr(0, line.size() - 1);
        }

        // =========================
        // IF GOTO
        // if a < b goto L1
        // =========================
        else if(line.find("if") == 0) {

            ins.type = IF_GOTO;

            std::stringstream ss(line);
            std::string tmp, gotoWord;

            ss >> tmp; // if
            ss >> ins.src1 >> ins.op >> ins.src2;
            ss >> gotoWord >> ins.label;
        }

        // =========================
        // GOTO
        // =========================
        else if(line.find("goto") == 0) {

            ins.type = GOTO;

            std::stringstream ss(line);
            std::string tmp;

            ss >> tmp >> ins.label;
        }

        // =========================
        // NORMAL INSTRUCTION
        // t1 = a + b
        // =========================
       else
{
    ins.type = BINARY;

    std::stringstream ss(line);
    std::string eq;

    ss >> ins.dest >> eq >> ins.src1 >> ins.op >> ins.src2;

    // REMOVE invalid parsing cases
    if(ins.dest == "" || ins.src1 == "" || ins.op == "" || ins.src2 == "")
        continue;

    // usage count (ignore numbers)
    if(!isdigit(ins.src1[0])) usageCount[ins.src1]++;
    if(!isdigit(ins.src2[0])) usageCount[ins.src2]++;
    if(!isdigit(ins.dest[0])) usageCount[ins.dest]++;
}

        instructions.push_back(ins);
    }

    return instructions;
}