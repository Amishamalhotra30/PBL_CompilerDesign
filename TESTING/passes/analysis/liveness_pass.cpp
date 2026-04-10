#include "liveness_pass.h"

void LivenessPass::run(CFG& cfg) {

    bool changed = true;

    while(changed){
        changed = false;

        for(int i = cfg.blocks.size()-1; i>=0; i--){

            auto& B = cfg.blocks[i];

            auto oldIN = B.IN;
            auto oldOUT = B.OUT;

            // CLEAR OUT FIRST
            B.OUT.clear();

            // OUT = union(successor IN)
            for(int succ : cfg.edges[i]){
                B.OUT.insert(
                    cfg.blocks[succ].IN.begin(),
                    cfg.blocks[succ].IN.end()
                );
            }

            // IN = USE ∪ (OUT - DEF)
            B.IN = B.USE;

            for(auto &v : B.OUT){
                if(B.DEF.find(v)==B.DEF.end())
                    B.IN.insert(v);
            }

            if(B.IN != oldIN || B.OUT != oldOUT)
                changed = true;
        }
    }
}