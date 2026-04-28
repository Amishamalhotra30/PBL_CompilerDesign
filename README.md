# PBL_CompilerDesign
# Graph Coloring Register Allocation Visualizer

## 1. Introduction

This project implements a compiler backend simulation focusing on **register allocation using graph coloring**. It demonstrates how a compiler efficiently assigns variables to a limited number of CPU registers while minimizing memory usage through spill handling.

The system takes **Three Address Code (TAC)** as input and processes it through multiple compiler phases, producing both analytical outputs and graphical visualizations.

---

## 2. Objectives

- To simulate the register allocation phase of a compiler
- To implement graph coloring for interference graph-based allocation
- To analyze register pressure and spill decisions
- To compare performance across different hardware architectures
- To provide visual representations of compiler internals

---



## 3. Features

### 3.1 Compiler Pipeline
The project implements the following stages:

- TAC parsing and instruction representation  
- Basic block construction  
- Control Flow Graph (CFG) generation  
- Liveness analysis  
- Interference graph construction  
- Graph coloring register allocation  
- Spill handling using cost-based strategy  
- Target code generation  

---

### 3.2 Architecture Comparison

The system supports two configurations:

- Embedded System (4 registers)  
- Desktop System (8 registers)  

It compares:

- Number of spills  
- Register pressure  
- Allocation efficiency  

---

### 3.3 Spill Handling

Spill decisions are made using a cost function:
spill_cost = usage / degree

Where:
- `usage` = number of times a variable is used  
- `degree` = number of interfering variables  

The system identifies variables with lower cost as better spill candidates.

---

### 3.4 Visualization

The project generates graphical representations using Graphviz:

- Control Flow Graph (CFG)  
- Interference Graph (Embedded)  
- Interference Graph (Desktop)  

Nodes are color-coded based on register assignment and spill status.

---

### 3.5 Web Interface

A Flask-based interface allows users to:

- Upload TAC files  
- Execute the compiler pipeline  
- View graphs and outputs  
- Analyze spill behavior and costs  

---

## 4. Project Structure
C:.
│   cfg.dot
│   cfg.png
│   desktop.png
│   Desktop_interference.dot
│   embedded.png
│   Embedded_interference.dot
│   graph.png
│   interference.dot
│   interference.png
│   main.cpp
│   main.exe
│   run.bat
│   tempCodeRunnerFile.bat
│   
├───.vscode
│       c_cpp_properties.json
│       
├───backend
│       instruction_emitter.cpp
│       target_codegen.cpp
│       target_codegen.h
│       
├───core
│       architecture.h
│       basic_block.h
│       cfg.h
│       instruction.h
│       
├───input
│       branch.tac
│       complex.tac
│       forced.tac
│       high_registr.tac
│       medium.tac
│       merge.tac
│       sample1.tac
│       stress.tac
│       
├───parser
│       tac_parser.cpp
│       tac_parser.h
│       
├───passes
│   ├───allocation
│   │       graph_coloring_pass.cpp
│   │       graph_coloring_pass.h
│   │       interference_graph_pass.cpp
│   │       interference_graph_pass.h
│   │       
│   ├───analysis
│   │       basic_block_pass.cpp
│   │       basic_block_pass.h
│   │       cfg_builder_pass.cpp
│   │       cfg_builder_pass.h
│   │       liveness_pass.cpp
│   │       liveness_pass.h
│   │       register_pressure_pass.cpp
│   │       register_pressure_pass.h
│   │       
│   ├───architecture
│   │       architecture_manager.h
│   │       
│   └───optimization
│           peephole_pass.cpp
│           peephole_pass.h
│           
├───server
│       app.py
│       cfg.dot
│       Desktop_interference.dot
│       Embedded_interference.dot
│       
├───ui
│       cfg.png
│       desktop.png
│       embedded.png
│       index.html
│       
├───utils
└───visualization
        cfg_exporter.cpp
        cfg_exporter.h
        graphviz_exporter.cpp
        graphviz_exporter.h

---

## 5. Technologies Used

- C++ for compiler implementation  
- Python (Flask) for backend server  
- HTML, CSS, JavaScript for frontend  
- Graphviz for graph visualization  

---

## 6. Installation and Setup

### Step 1: Compile the C++ Code

```bash
g++ main.cpp graph_coloring_pass.cpp target_codegen.cpp -o main.exe
Ensure all required source files are included.

Step 2: Run the Backend Server
cd server
python app.py
Step 3: Launch the Application

Open a browser and go to:

http://127.0.0.1:5000
7. Usage
Upload a TAC file through the interface
Click "Run Compiler"
View:
Control Flow Graph
Interference graphs
Register allocation results
Spill analysis and cost breakdown
Generated target code
8. Sample Input
t1 = a + b
t2 = c + d
t3 = t1 * t2
t4 = e + f
t5 = g + h
t6 = t4 * t5
t7 = t3 + t6
9. Output Description

The system produces:

Register allocation mapping
Spill decisions with cost values
Assembly-like target code
Optimization report including:
Register pressure
Total spills
10. Key Concepts Implemented
Graph Coloring Algorithm
Interference Graph
Liveness Analysis
Register Allocation Strategies
Spill Cost Optimization
Compiler Backend Design

11. Future Enhancements
Interactive graph visualization
Support for additional architectures
Static Single Assignment (SSA) form
Advanced compiler optimizations
Performance benchmarking tools

12. Conclusion
This project demonstrates the practical implementation of register allocation using graph coloring. It highlights the trade-offs between register usage and memory spills while providing both analytical and visual insights into compiler behavior.
