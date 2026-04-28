#include "cfg_exporter.h"
#include <fstream>
#include <sstream>
#include "../core/cfg.h"

std::string instructionToString(const Instruction &ins)
{
    if(ins.type == LABEL)
        return ins.label + ":";

    else if(ins.type == GOTO)
        return "goto " + ins.label;

    else if(ins.type == IF_GOTO)
        return "if " + ins.src1 + " " + ins.op + " " + ins.src2 + " goto " + ins.label;

    else
        return ins.dest + " = " + ins.src1 + " " + ins.op + " " + ins.src2;
}

void exportCFG(CFG &cfg)
{
    std::ofstream out("cfg.dot");

    out << "digraph CFG {\n";

    // 🔥 GLOBAL STYLE
    out << "node [shape=box, style=\"filled,rounded\", fontname=\"Arial\"];\n";
    out << "edge [color=black, penwidth=1.5];\n";

    // 🔥 TITLE
    out << "label=\"Control Flow Graph\";\n";
    out << "labelloc=top;\n";
    out << "fontsize=20;\n";

    // =============================
    // NODES
    // =============================
    for(auto &b : cfg.blocks)
    {
        std::stringstream label;

        label << "Block " << b.id << "\\n";

        for(auto &ins : b.instructions)
        {
            label << instructionToString(ins) << "\\n";
        }

        // 🔥 COLOR LOGIC
        std::string color = "lightblue"; // default

        if(b.id == 0)
            color = "lightgreen";   // Entry block

        else if(cfg.edges[b.id].empty())
            color = "lightpink";    // Exit block

        out << "B" << b.id
            << " [fillcolor=" << color
            << ", label=\"" << label.str() << "\"];\n";
    }

    // =============================
    // EDGES
    // =============================
    for(auto &e : cfg.edges)
    {
        for(auto &to : e.second)
        {
            out << "B" << e.first
                << " -> B" << to << ";\n";
        }
    }

    // =============================
    // LEGEND (🔥 VERY IMPORTANT)
    // =============================
    out << "subgraph cluster_legend {\n";
    out << "label=\"Legend\";\n";

    out << "entry [label=\"Entry Block\", style=filled, fillcolor=lightgreen];\n";
    out << "normal [label=\"Normal Block\", style=filled, fillcolor=lightblue];\n";
    out << "exit [label=\"Exit Block\", style=filled, fillcolor=lightpink];\n";

    out << "}\n";

    out << "}\n";
    out.close();
}