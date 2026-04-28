g++ main.cpp ^
parser/tac_parser.cpp ^
passes/analysis/basic_block_pass.cpp ^
passes/analysis/cfg_builder_pass.cpp ^
passes/analysis/liveness_pass.cpp ^
passes/allocation/interference_graph_pass.cpp ^
passes/allocation/graph_coloring_pass.cpp ^
passes/optimization/peephole_pass.cpp ^
backend/target_codegen.cpp ^
passes/analysis/register_pressure_pass.cpp ^
visualization/graphviz_exporter.cpp ^
visualization/cfg_exporter.cpp ^
-o main.exe