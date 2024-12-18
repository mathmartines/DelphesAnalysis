#ifndef ATLAS13TEVDITAU_SELECTIONS_H
#define ATLAS13TEVDITAU_SELECTIONS_H

#include <iostream>
#include <vector>
#include <variant>
#include <math.h>
#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "classes/DelphesClasses.h"
#include "DelphesAnalysis/ObjectSelection.h"
#include "DelphesAnalysis/Cut.h"
#include "DelphesAnalysis/Observable.h"
#include "DelphesAnalysis/EventData.h"

/// ------------------------- Particle Selections -------------------------

/// @brief - Electrons candidates must have |eta| < 2.47 and not within 1.37 < |eta| < 1.52
class ElectronCandidatesATLAS: public ObjectSelection {
    public:
        void selectObjects(EventData* event_data) const override;
};

/// @brief - Muon candidates must have |eta| < 2.5
class MuonCandidatesATLAS: public ObjectSelection {
    public:
        void selectObjects(EventData* event_data) const override;
};

/// @brief - Jets must have pT > 20 and |eta| < 2.5
class JetsATLAS: public ObjectSelection {
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

// /// ------------------------------ Cuts --------------------------------

/// @brief - no leptons in the event
class NumberOfLeptons: public Cut {
    public:
        NumberOfLeptons(int number): number_of_lep(number) {};
        bool selectEvent(EventData* event_data) const override;
    private:
        /// @brief - stores the number of leptons we must have in the event
        int number_of_lep;
};

/// @brief - cuts on the number of hadronic taus in the event
class NumberOfTauHad: public Cut {
    public: 
        NumberOfTauHad(int min): min_taus(min) {};
        bool selectEvent(EventData* event_data) const override;
    private:
        /// minimum number of hadronic taus in the event
        int min_taus;
};

/// @brief - cuts on the pT of the hadronic taus
///          check if we have a pair of opposite charge tau candidates
///          checl if the taus are back to back
class HadronicTausCut: public Cut {
    public:
        HadronicTausCut(): obs_deltaphi(), number_of_taus_cut(2) {};
        bool selectEvent(EventData* event_data) const override;
    private: 
        /// @brief - selects taus with pT greater than 125 GeV
        void selectTaus(EventData* event_data) const;
        /// @brief - checks if the selected hadronic taus have opposite charge 
        bool oppositeCharge(EventData* event_data) const;
        /// @brief - calculates the angular diff
       const DeltaPhi obs_deltaphi;
       const NumberOfTauHad number_of_taus_cut;
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


/// @brief - checks if the hadronic tau has the oppostite charge of the lepton
///          checks if the lepton and hadronic tau are back to back
///          checks if the transverse mass of the missing energy and lepton is less than 40GeV
///          checks if the invariant mass of the hadronic tau and lepton is not inside the Z mass window
// class TauLeptonEventsCuts: public Cut {
//     public: 
//         TauLeptonEventsCuts(): delta_phi(), mt(), mtatlas(), m() {};
//         bool selectEvent(EventData* event_data) const override;
    
//     private:
//         /// @brief - checks if there's a tau with |eta| < 2.3 with opposite electric charge of the lepton 
//         bool oppositeChargeHadTau(EventData* event_data, int lepton_charge) const; 
//         /// @brief - calculates the observables
//         const DeltaPhi delta_phi;
//         const TransverseMass mt;
//         const TransverseMassATLAS mtatlas;
//         const InvariantMass m;
// };

/// @brief Defining the observable
class TransverseMassDiTauHad: public EventObservable {
    public:
        double evaluateObservable(const EventData* event_data) const override;
};

#endif