@echo off

echo Compiling...
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

echo Running program...
main.exe

echo Generating CFG graph...
"C:\Users\amish\Downloads\windows_10_cmake_Release_Graphviz-14.1.4-win32\Graphviz-14.1.4-win32\bin\dot.exe" -Tpng cfg.dot -o cfg.png

echo Generating Interference graphs...

"C:\Users\amish\Downloads\windows_10_cmake_Release_Graphviz-14.1.4-win32\Graphviz-14.1.4-win32\bin\dot.exe" -Tpng Embedded_interference.dot -o embedded.png

"C:\Users\amish\Downloads\windows_10_cmake_Release_Graphviz-14.1.4-win32\Graphviz-14.1.4-win32\bin\dot.exe" -Tpng Desktop_interference.dot -o desktop.png

echo Opening graphs...

explorer "%cd%\cfg.png"
explorer "%cd%\embedded.png"
explorer "%cd%\desktop.png"