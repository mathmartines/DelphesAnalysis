#ifndef CMS_MONOLEP_H
#define CMS_MONOLEP_H

#include "DelphesAnalysis/EventData.h"

class EventDataCMS_monolepton: public EventData {
    public:
        void setBranches () override {
            addBranch("Electron");
            addBranch("Muon");
            addBranch("Jet");
            addBranch("MissingET");
            /// vectors to store the particles for the analysis
            addParticlesVector("Electron");
            addParticlesVector("Muon");
            addParticlesVector("Jet");
        };
};


#endif