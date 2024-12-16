#ifndef EVENT_DATA_CMS_DILEPTON
#define EVENT_DATA_CMS_DILEPTON

#include "DelphesAnalysis/EventData.h"

/// @brief - stores the data we need for dielectron analysis
class EventDataCMS: public EventData {
    public:
        void setBranches () override {
            addBranch("Electron");
            addBranch ("Muon");
            addBranch ("Track");
            addParticlesVector("Electron");
            addParticlesVector ("Muon");
        };
};

#endif