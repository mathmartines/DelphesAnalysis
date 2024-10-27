#ifndef EVENT_DATA_CMS_DILEPTON
#define EVENT_DATA_CMS_DILEPTON

#include "DelphesAnalysis/EventData.h"

/// @brief - stores the data we need for dielectron analysis
class EventDataCMS_dielectron: public EventData {
    public:
        void setBranches () override {
            addBranch("Electron");
            addParticlesVector("Electron");
        };
};

class EventDataCMS_dimuon: public EventData {
    public:
        void setBranches () override {
            addBranch("Muon");
            addBranch("Track");
            addParticlesVector("Muon");
        };
};

#endif