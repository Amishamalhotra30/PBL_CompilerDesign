#pragma once
#include <string>

enum InstrType {
    ASSIGN,     // x = y
    BINARY,     // x = y op z
    IF_GOTO,    // if x < y goto L1
    GOTO,       // goto L1
    LABEL       // L1:
};

struct Instruction {
    InstrType type;

    std::string op;     // +, -, *, <, etc
    std::string dest;   // result
    std::string src1;
    std::string src2;
    std::string label;  // for goto / label

    Instruction() = default;

    // For normal instructions
    Instruction(InstrType t, std::string o,
                std::string d,
                std::string s1,
                std::string s2,
                std::string lbl = "")
        : type(t), op(o), dest(d),
          src1(s1), src2(s2), label(lbl) {}
};