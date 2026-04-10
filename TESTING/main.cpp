#include <iostream>

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
using namespace std;
int main(){

    // =============================
    // STEP 1 — PARSE TAC
    // =============================
    TACParser parser;
    cout << "Choose input file:\n";
cout << "1. complex.tac\n";
cout << "2. branch.tac\n";
cout << "3. sample1.tac\n";
cout << "4. forced.tac\n";
cout << "5. high_register.tac\n";
cout << "6. medium.tac\n";
cout << "7. stress.tac\n";
cout << "8. merge.tac\n";


int choice;
cin >> choice;

string path;

if(choice == 1) path = "input/complex.tac";
else if(choice == 2) path = "input/branch.tac";
else if(choice == 3) path = "input/sample1.tac";
else if(choice == 4) path = "input/forced.tac";
else if(choice == 5) path = "input/high_register.tac";
else if(choice == 6) path = "input/medium.tac";
else if(choice == 7) path = "input/stress.tac";
else path = "input/merge.tac";

auto program = parser.parse(path);
    // =============================
    // STEP 1.5 — PRINT TAC
    // =============================
    cout << "\nParsed Instructions:\n";
    for(auto &ins : program)
    {
        if(ins.type == LABEL)
            cout << ins.label << ":\n";

        else if(ins.type == GOTO)
            cout << "goto " << ins.label << "\n";

        else if(ins.type == IF_GOTO)
            cout << "if " << ins.src1 << " "
                      << ins.op << " "
                      << ins.src2 << " goto "
                      << ins.label << "\n";

        else
            cout << ins.dest << " = "
                      << ins.src1 << " "
                      << ins.op << " "
                      << ins.src2 << "\n";
    }

    // =============================
    // STEP 2 — BASIC BLOCKS
    // =============================
    BasicBlockPass bbPass;
    auto blocks = bbPass.run(program);

    cout << "\nBasic Blocks:\n";
    for (auto &b : blocks) {

        cout << "Block " << b.id << "\n";

        for (auto &ins : b.instructions) {

            if(ins.type == LABEL)
                cout << ins.label << ":\n";

            else if(ins.type == GOTO)
                cout << "goto " << ins.label << "\n";

            else if(ins.type == IF_GOTO)
                cout << "if " << ins.src1 << " "
                          << ins.op << " "
                          << ins.src2 << " goto "
                          << ins.label << "\n";

            else
                cout << ins.dest << " = "
                          << ins.src1 << " "
                          << ins.op << " "
                          << ins.src2 << "\n";
        }

        cout << "USE: ";
        for(auto &v : b.USE) cout << v << " ";

        cout << "\nDEF: ";
        for(auto &v : b.DEF) cout << v << " ";

        cout << "\n\n";
    }

    // =============================
    // STEP 3 — CFG
    // =============================
    CFGBuilderPass cfgPass;
    CFG cfg = cfgPass.run(blocks);

    cout << "\nCFG Edges:\n";
    for(auto &e : cfg.edges){
        cout << e.first << " -> ";
        for(auto &x : e.second)
            cout << x << " ";
        cout << "\n";
    }

    // =============================
    // STEP 4 — LIVENESS
    // =============================
    LivenessPass livePass;
    livePass.run(cfg);

    cout << "\nLiveness:\n";
    for(auto &b : cfg.blocks){
        cout << "Block " << b.id << "\n";

        cout << "IN: ";
        for(auto &v : b.IN) std::cout << v << " ";

        cout << "\nOUT: ";
        for(auto &v : b.OUT) std::cout << v << " ";

        cout << "\n\n";
    }

    // =============================
    // STEP 5 — INTERFERENCE GRAPH
    // =============================
    InterferenceGraphPass igPass;
    igPass.run(cfg);

    cout << "\nInterference Graph:\n";
    for(auto &node : igPass.graph)
    {
        cout << node.first << " : ";
        for(auto &nbr : node.second)
            std::cout << nbr << " ";
        cout << "\n";
    }

    // =============================
    // STEP 6 — CFG VISUALIZATION
    // =============================
    exportCFG(cfg);
    cout << "\nCFG exported to cfg.dot\n";

    // =============================
    // STEP 7 — ARCHITECTURE LOOP
    // =============================
    vector<Architecture> archs = {
        ArchitectureManager::embedded(),
        ArchitectureManager::desktop()
    };

    for(auto &arch : archs)
    {
        vector<std::string> registers;

        for(auto &r : arch.fastRegs)
            registers.push_back(r);

        for(auto &r : arch.slowRegs)
            registers.push_back(r);

        cout << "\n=============================\n";
        cout << "Architecture: " << arch.name << "\n";
        cout << "Registers Available: "
                  << registers.size() << "\n";

        // =============================
        // STEP 8 — GRAPH COLORING
        // =============================
        GraphColoringPass colorPass;

        colorPass.run(igPass.graph, registers, parser.usageCount);

        cout << "\nRegister Allocation:\n";
        for(auto &p : colorPass.allocation)
        {
            if(p.second == "SPILL")
                cout << p.first << " -> SPILLED TO MEMORY\n";
            else
                cout << p.first << " -> " << p.second << "\n";
        }

        // =============================
        // STEP 9 — INTERFERENCE GRAPH VISUALIZATION
        // =============================
        GraphvizExporter exporter;
        exporter.exportInterferenceGraph(
            igPass.graph,
            colorPass.allocation,
            arch.name + "_interference.dot"
        );

        cout << "Interference graph exported for "
                  << arch.name << "\n";

        // =============================
        // STEP 10 — TARGET CODEGEN
        // =============================
        TargetCodegen codegen;  
        codegen.generate(program, colorPass.allocation);

        // =============================
        // STEP 11 — OPTIMIZATION REPORT
        // =============================
        int pressure = RegisterPressurePass::compute(cfg);

        int spills = 0;
        for(auto &p : colorPass.allocation)
            if(p.second == "SPILL")
                spills++;

        cout << "\nOptimization Report\n";
        cout << "Available Registers: "
                  << registers.size() << "\n";

        cout << "Maximum Register Pressure: "
                  << pressure << "\n";

        cout << "Spills Generated: "
                  << spills << "\n";
    }

    cout << "\nPipeline executed successfully\n";

    return 0; 
}