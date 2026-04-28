#include "target_codegen.h"
#include <iostream>

void TargetCodegen::generate(
    std::vector<Instruction>& program,
    std::unordered_map<std::string,std::string>& allocation)
{
    std::cout << "\nGenerated Target Code:\n";

    for(auto &ins : program)
    {
        // =========================
        // LABEL
        // =========================
        if(ins.type == LABEL)
        {
            std::cout << ins.label << ":\n";
            continue;
        }

        // =========================
        // GOTO
        // =========================
        if(ins.type == GOTO)
        {
            std::cout << "JMP " << ins.label << "\n";
            continue;
        }

        // =========================
        // IF GOTO
        // =========================
        if(ins.type == IF_GOTO)
        {
            std::string src1 = allocation.count(ins.src1) ?
                               allocation[ins.src1] : ins.src1;

            std::string src2;

            if(isdigit(ins.src2[0]))
                src2 = ins.src2;
            else
                src2 = allocation.count(ins.src2) ?
                       allocation[ins.src2] : ins.src2;

            // 🔥 handle spill
            if(src1 == "SPILL") src1 = "MEM";
            if(src2 == "SPILL") src2 = "MEM";

            std::cout << "CMP " << src1 << ", " << src2 << "\n";
            std::cout << "J" << ins.op << " " << ins.label << "\n";
            continue;
        }

        // =========================
        // NORMAL INSTRUCTION
        // =========================
        std::string dest = allocation.count(ins.dest) ?
                           allocation[ins.dest] : ins.dest;

        std::string src1 = allocation.count(ins.src1) ?
                           allocation[ins.src1] : ins.src1;

        std::string src2 = allocation.count(ins.src2) ?
                           allocation[ins.src2] : ins.src2;

        // 🔥 handle spills clearly
        if(dest == "SPILL") dest = "MEM";
        if(src1 == "SPILL") src1 = "MEM";
        if(src2 == "SPILL") src2 = "MEM";

        if(ins.op == "+")
            std::cout << "ADD " << dest << ", " << src1 << ", " << src2 << "\n";

        else if(ins.op == "*")
            std::cout << "MUL " << dest << ", " << src1 << ", " << src2 << "\n";

        else
            std::cout << "MOV " << dest << ", " << src1 << "\n";
    }
}