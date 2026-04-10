#pragma once
#include "../../core/architecture.h"

class ArchitectureManager {
public:

    static Architecture embedded()
    {
        Architecture a;
        a.name = "Embedded";
        a.fastRegs = {"R1","R2"};
        a.slowRegs = {"R3","R4"};
        return a;
    }

    static Architecture desktop()
    {
        Architecture a;
        a.name = "Desktop";
        a.fastRegs = {"R1","R2","R3","R4","R5","R6"};
        a.slowRegs = {"R7","R8"};
        return a;
    }
};