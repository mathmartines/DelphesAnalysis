#ifndef EVENT_DATA_ATLAS_H
#define EVENT_DATA_ATLAS_H

#include "DelphesAnalysis/EventData.h"

class EventDataATLAS: public EventData {

    public:
        /// @brief - set the tree reader pointer to extract the data from
        void setBranches() override {
            /// branches needed 
            addBranch("Jet");
            addBranch("Electron");
            addBranch("Muon");
            addBranch("MissingET");
            addBranch("Track");
            /// defining the vectors to hold the particles for the analysis
            addParticlesVector("Electron");
            addParticlesVector("Muon");
            addParticlesVector("Jet");
            addParticlesVector("HadronicTau");
        };       
};

#endif