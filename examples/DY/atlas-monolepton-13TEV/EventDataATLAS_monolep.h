#ifndef EVENT_DATA_ATLAS_MONOLEP_H
#define EVENT_DATA_ATLAS_MONOLEP_H

#include "DelphesAnalysis/EventData.h"


class EventDataATLAS_monolep: public EventData {
    public:
        void setBranches() override {
            addBranch ("Electron");
            addBranch ("Muon");
            addBranch ("Jet");
            addBranch ("MissingET");
            addParticlesVector ("Electron");
            addParticlesVector ("Muon");
            addParticlesVector ("Jet");
        };
};


#endif