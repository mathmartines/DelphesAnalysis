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
#include "DelphesAnalysis/Observable.h"

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
///          check if we have a pair of opposite charge tau candidates
///          checl if the taus are back to back
class HadronicTausCut: public Cut {
    public:
        HadronicTausCut() = default;
        bool selectEvent(EventData* event_data) const override;
    private: 
        /// @brief - checks if the selected hadronic taus have opposite charge 
        bool oppositeCharge(EventData* event_data) const;
        /// @brief - calculates the angular diff
       const DeltaPhi obs_deltaphi;
};

/// @brief - veto on events with jets tagged as containing a b
class bVeto: public Cut {
    public:
        bool selectEvent(EventData* event_data) const override;
};

/// @brief - selects events only if there's at least one b-tagged jet
class bTag: public Cut {
    public: 
        bool selectEvent(EventData* event_data) const override;
};

/// @brief - cuts on the leptons pT
///        - assumes we only have one lepton
class LeptonPtCut: public Cut{
    public: 
        bool selectEvent(EventData* event_data) const override;
};

/// @brief - selects events with only one lepton
class SingleLeptonCut: public Cut {
    public:
        bool selectEvent(EventData* event_data) const override;
};

/// @brief - selects events with at least one tau
class ContainsHadronicTaus: public Cut {
    public:
        bool selectEvent(EventData* event_data) const override {return event_data->hadronic_taus.size() > 0;};
};

/// @brief - checks if the hadronic tau has the oppostite charge of the lepton
///          checks if the lepton and hadronic tau are back to back
///          checks if the transverse mass of the missing energy and lepton is less than 40GeV
///          checks if the invariant mass of the hadronic tau and lepton is not inside the Z mass window
class TauLeptonEventsCuts: public Cut {
    public: 
        bool selectEvent(EventData* event_data) const override;
    
    private:
        /// @brief - checks if there's a tau with |eta| < 2.3 with opposite electric charge of the lepton 
        bool oppositeChargeHadTau(EventData* event_data, int lepton_charge) const; 
        /// @brief - calculates the observables
        const DeltaPhi delta_phi;
        const TransverseMass mt;
        const TransverseMassATLAS mtatlas;
        const InvariantMass m;
};




#endif