#ifndef DITAUATLAS_H
#define DITAUATLAS_H

#include <iostream>
#include <vector>
#include "classes/DelphesClasses.h"
#include "DelphesAnalysis/Cut.h"

//// Cuts for the tau-hadronic tau-hadronic channel

/// @brief - no leptons in the event
class LeptonsVeto: public Cut {
    public:
        bool selectEvent(EventData* event_data) const override {
            return event_data->electrons.size() + event_data->muons.size() == 0;    
        };
};

/// @brief - cuts on the pT of the hadronic taus
class HadronicTausCut: public Cut {
    public:
        bool selectEvent(EventData* event_data) const override;
    private: 
        /// @brief - checks if the selected hadronic taus have opposite charge 
        bool oppositeCharge(EventData* event_data) const;
        /// @brief - checks if the tau candidates are back to back in the transverse plane
        bool backToBack(EventData* event_data) const;
};

/// @brief - veto on events with jets tagged as containing a b
class bVeto: public Cut {
    public:
        bool selectEvent(EventData* event_data) const override;
};



#endif