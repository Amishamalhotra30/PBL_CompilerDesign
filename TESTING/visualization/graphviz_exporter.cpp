#include "graphviz_exporter.h"
#include <fstream>

void GraphvizExporter::exportInterferenceGraph(
    const std::unordered_map<std::string, std::set<std::string>>& graph,
    const std::unordered_map<std::string, std::string>& allocation,
    const std::string& filename
){
    std::ofstream out(filename);

    out << "graph G {\n";

out << "layout=neato;\n";
out << "overlap=false;\n";
out << "splines=false;\n";   
out << "sep=1.0;\n";

out << "node [shape=circle, style=filled, fontname=\"Arial\", fixedsize=true, width=1.2];\n";
out << "edge [color=gray, penwidth=1.2];\n";
    // =============================
    // PRINT ALL NODES
    // =============================
    for(const auto &node : graph)
    {
        std::string var = node.first;
        std::string color = "lightgray";

        if(allocation.count(var))
        {
            std::string reg = allocation.at(var);

            if(reg == "R1") color = "lightgreen";
else if(reg == "R2") color = "lightblue";
else if(reg == "R3") color = "lightyellow";
else if(reg == "R4") color = "lightpink";
else if(reg == "R5") color = "orange";
else if(reg == "R6") color = "violet";
else if(reg == "R7") color = "cyan";
else if(reg == "R8") color = "gold";
else if(reg == "SPILL") color = "lightgray";
        }

        out << "\"" << var << "\" [label=\""
            << var;

        if(allocation.count(var))
            out << " (" << allocation.at(var) << ")";

        out << "\", fillcolor=" << color << "];\n";
    }

    // =============================
    //  PRINT EDGES (FIXED)
    // =============================
    for(const auto &node : graph)
    {
        const std::string &u = node.first;

        for(const auto &v : node.second)
        {
            if(u < v)   // THIS LINE FIXES EVERYTHING
            {
                out << "\"" << u << "\" -- \"" << v << "\";\n";
            }
        }
    }

    out << "}\n";
    out.close();
}