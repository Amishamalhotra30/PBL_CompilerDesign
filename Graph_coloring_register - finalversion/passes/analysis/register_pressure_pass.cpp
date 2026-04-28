#include "register_pressure_pass.h"

int RegisterPressurePass::compute(CFG &cfg)
{
    int maxPressure = 0;

    for(auto &block : cfg.blocks)
    {
        int pressure = block.IN.size();

        if(pressure > maxPressure)
            maxPressure = pressure;
    }

    return maxPressure;
}