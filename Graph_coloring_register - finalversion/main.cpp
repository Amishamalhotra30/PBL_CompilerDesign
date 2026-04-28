#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

#include "parser/tac_parser.h"
#include "passes/analysis/basic_block_pass.h"
#include "passes/analysis/cfg_builder_pass.h"
#include "passes/analysis/liveness_pass.h"
#include "passes/allocation/interference_graph_pass.h"
#include "passes/allocation/graph_coloring_pass.h"
#include "passes/architecture/architecture_manager.h"
#include "backend/target_codegen.h"
#include "passes/optimization/peephole_pass.h"
#include "passes/analysis/register_pressure_pass.h"
#include "visualization/graphviz_exporter.h"
#include "visualization/cfg_exporter.h"

int main(int argc, char* argv[])
{
    // =============================
    // ✅ STEP 1 — INPUT
    // =============================
    if(argc < 2)
    {
        std::cout << "Error: No input file provided\n";
        std::cout << "Usage: main.exe <file.tac>\n";
        return 1;
    }

    std::string path = argv[1];
    std::cout << "[INFO] Using file: " << path << "\n";

    TACParser parser;
    auto program = parser.parse(path);

    // =============================
    // STEP 1.5 — PRINT TAC
    // =============================
    std::cout << "\nParsed Instructions:\n";
    for(auto &ins : program)
    {
        if(ins.type == LABEL)
            std::cout << ins.label << ":\n";

        else if(ins.type == GOTO)
            std::cout << "goto " << ins.label << "\n";

        else if(ins.type == IF_GOTO)
            std::cout << "if " << ins.src1 << " "
                      << ins.op << " "
                      << ins.src2 << " goto "
                      << ins.label << "\n";

        else
            std::cout << ins.dest << " = "
                      << ins.src1 << " "
                      << ins.op << " "
                      << ins.src2 << "\n";
    }

    // =============================
    // STEP 2 — BASIC BLOCKS
    // =============================
    BasicBlockPass bbPass;
    auto blocks = bbPass.run(program);

    std::cout << "\nBasic Blocks:\n";
    for (auto &b : blocks)
    {
        std::cout << "Block " << b.id << "\n";

        for (auto &ins : b.instructions)
        {
            if(ins.type == LABEL)
                std::cout << ins.label << ":\n";

            else if(ins.type == GOTO)
                std::cout << "goto " << ins.label << "\n";

            else if(ins.type == IF_GOTO)
                std::cout << "if " << ins.src1 << " "
                          << ins.op << " "
                          << ins.src2 << " goto "
                          << ins.label << "\n";

            else
                std::cout << ins.dest << " = "
                          << ins.src1 << " "
                          << ins.op << " "
                          << ins.src2 << "\n";
        }

        std::cout << "USE: ";
        for(auto &v : b.USE) std::cout << v << " ";

        std::cout << "\nDEF: ";
        for(auto &v : b.DEF) std::cout << v << " ";

        std::cout << "\n\n";
    }

    // =============================
    // STEP 3 — CFG
    // =============================
    CFGBuilderPass cfgPass;
    CFG cfg = cfgPass.run(blocks);

    std::cout << "\nCFG Edges:\n";
    for(auto &e : cfg.edges)
    {
        std::cout << e.first << " -> ";
        for(auto &x : e.second)
            std::cout << x << " ";
        std::cout << "\n";
    }

    // =============================
    // STEP 4 — LIVENESS
    // =============================
    LivenessPass livePass;
    livePass.run(cfg);

    std::cout << "\nLiveness:\n";
    for(auto &b : cfg.blocks)
    {
        std::cout << "Block " << b.id << "\n";

        std::cout << "IN: ";
        for(auto &v : b.IN) std::cout << v << " ";

        std::cout << "\nOUT: ";
        for(auto &v : b.OUT) std::cout << v << " ";

        std::cout << "\n\n";
    }

    // =============================
    // STEP 5 — INTERFERENCE GRAPH
    // =============================
    InterferenceGraphPass igPass;
    igPass.run(cfg);

    std::cout << "\nInterference Graph:\n";
    for(auto &node : igPass.graph)
    {
        std::cout << node.first << " : ";
        for(auto &nbr : node.second)
            std::cout << nbr << " ";
        std::cout << "\n";
    }

    // =============================
    // STEP 6 — CFG VISUALIZATION
    // =============================
    exportCFG(cfg);
    std::cout << "\nCFG exported to cfg.dot\n";

    // =============================
    // STEP 7 — ARCHITECTURE LOOP
    // =============================
    std::vector<Architecture> archs = {
        ArchitectureManager::embedded(),
        ArchitectureManager::desktop()
    };

    for(auto &arch : archs)
    {
        std::vector<std::string> registers;

        for(auto &r : arch.fastRegs)
            registers.push_back(r);

        for(auto &r : arch.slowRegs)
            registers.push_back(r);

        std::cout << "\n=============================\n";
        std::cout << "Architecture: " << arch.name << "\n";
        std::cout << "Registers Available: "
                  << registers.size() << "\n";

        // =============================
        // GRAPH COLORING
        // =============================
        GraphColoringPass colorPass;
        colorPass.run(igPass.graph, registers, parser.usageCount);

        // =============================
        // 🔥 FIXED REGISTER OUTPUT
        // =============================
        std::cout << "\nRegister Allocation:\n";
        for(auto &p : colorPass.allocation)
        {
            if(p.second == "SPILL")
            {
                double cost = colorPass.spillCost[p.first];

                std::cout << "[SPILL] " << p.first 
                          << " cost=" << cost << "\n";
            }
            else
            {
                std::cout << p.first << " -> " << p.second << "\n";
            }
        }

        // =============================
        // GRAPH EXPORT
        // =============================
        GraphvizExporter exporter;
        exporter.exportInterferenceGraph(
            igPass.graph,
            colorPass.allocation,
            arch.name + "_interference.dot"
        );

        std::cout << "Interference graph exported for "
                  << arch.name << "\n";

        // =============================
        // TARGET CODE
        // =============================
        TargetCodegen codegen;
        codegen.generate(program, colorPass.allocation);

        // =============================
        // REPORT
        // =============================
        int pressure = RegisterPressurePass::compute(cfg);

        int spills = 0;
        for(auto &p : colorPass.allocation)
            if(p.second == "SPILL")
                spills++;

        std::cout << "\nOptimization Report\n";
        std::cout << "Available Registers: "
                  << registers.size() << "\n";

        std::cout << "Maximum Register Pressure: "
                  << pressure << "\n";

        std::cout << "Spills Generated: "
                  << spills << "\n";
    }

    std::cout << "\nPipeline executed successfully\n";

    return 0;
}