#ifndef ATLAS13TEVDITAU_SELECTIONS_H
#define ATLAS13TEVDITAU_SELECTIONS_H

#include <iostream>
#include <vector>
#include <math.h>
#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "classes/DelphesClasses.h"
#include "DelphesAnalysis/ObjectSelection.h"
#include "DelphesAnalysis/Cut.h"

/// ------------------------- Particle Selections -------------------------

/// @brief - Electrons candidates must have |eta| < 2.47 and not within 1.37 < |eta| < 1.52
class ElectronCandidates: public ObjectSelection {
    public:
        void selectObjects(EventData* event_data) const override;
};

/// @brief - Muon candidates must have |eta| < 2.5
class MuonCandidates: public ObjectSelection {
    public:
        void selectObjects(EventData* event_data) const override;
};

/// @brief - Jets must have pT > 20 and |eta| < 2.5
class Jets: public ObjectSelection {
    public:
        void selectObjects(EventData* event_data) const override;
};

/// @brief - Hadronic tau candidates are extracted from jets that have been tagged as tau jets
///          Also, must have |eta| < 2.5 without 1.37 < |eta| < 1.52.
///          One or three tracks
///          Charge +1 or -1
///          Important: I assume that the jets already have been selected for the analysis
class HadronicTaus: public ObjectSelection {
    public:
        void selectObjects(EventData* event_data) const override;
    private:
        /// @brief - counts the number of tracks around the hadronic tau within a radius equal to 0.2
        ///          the hadronic tau candidates must have only one or three associated tracks 
        bool countTracks(EventData* event_data, Jet* hadronic_tau) const;
};

/// ------------------------------ Cuts --------------------------------

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