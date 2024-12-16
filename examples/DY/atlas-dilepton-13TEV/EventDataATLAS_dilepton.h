#ifndef EVENT_DATA_ATLAS_DILEP_H
#define EVENT_DATA_ATLAS_DILEP_H

#include "DelphesAnalysis/EventData.h"


class EventDataATLAS_dileptons: public EventData {
    public:
        void setBranches() override {
            addBranch ("Electron");
            addBranch ("Muon");
            addBranch ("Track");
            addParticlesVector ("Electron");
            addParticlesVector ("Muon");
            addParticlesVector ("Jet");
        };
};


#endif