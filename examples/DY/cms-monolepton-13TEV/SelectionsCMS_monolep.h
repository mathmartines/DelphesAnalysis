#ifndef SELECTIONS_CMS_MONOLEP_H
#define SELECTIONS_CMS_MONOLEP_H

#include "TLorentzVector.h"
#include "classes/DelphesClasses.h"
#include "DelphesAnalysis/EventData.h"
#include "DelphesAnalysis/ObjectSelection.h"
#include "DelphesAnalysis/Cut.h"
#include "DelphesAnalysis/Observable.h"


/// @brief - Et > 35 GeV
///        - |eta| < 1.4 or 1.57 < |eta| < 2.5
///        - the Et sum in the calorimeter must be less than 3% of the lepton Et
class ElectronCandidatesCMS: public ObjectSelection {
    public:
        void selectObjects(EventData* event_data) const override;
};

/// @brief - |eta| < 2.4
///        - the sum scalar sum of the tracks must be less than 10% of the muon pt
class MuonCandidatesCMS: public ObjectSelection {
    public:
        void selectObjects(EventData* event_data) const override;
};

/// @brief - pT > 25 and |eta| < 2.5
class JetsCMS: public ObjectSelection {
    public:
        void selectObjects(EventData* event_data) const override;  
};

/// @brief - number of leptons must be grater or equal to 1
class NumberOfLeptons: public Cut {
    public:
        bool selectEvent(EventData* event_data) const override;
};

/// @brief - the leading lepton must have Pt > 200 GeV and |eta| < 2.5
class ElectronEvent: public Cut {
    public:
        bool selectEvent(EventData* event_data) const override;
};

/// @brief - leading muon must have pT > 50 GeV and |eta| < 2.4
class MuonEvent: public Cut {
    public:
        bool selectEvent(EventData* event_data) const override;
};

/// @brief - veto on events with additional leptons with pT > 25 GeV
class AdditionalLeptonsVeto: public Cut {
    public:
        AdditionalLeptonsVeto(bool electronEvents): eleEvents(electronEvents) {};
        bool selectEvent(EventData* event_data) const override;
    private: 
        bool eleEvents;
};

/// @brief - veto on events with more than 5 jets with Delta R (j, l) > 0.5
///        - only for muon events
class VetoOnJets: public Cut {
    public:
        bool selectEvent(EventData* event_data) const override;
    
};

/// @brief - veto on events where the leading jet is tagged as a b-jet
///        - only for muon events
class bJetVeto: public Cut {
    public:
        bool selectEvent (EventData* event_data) const override;
};


/// @brief - 0.4 < ptl/ptmin < 1.5
///        - Delta Phi > 2.5 (back to back)
class MissingETCuts: public Cut {
    public: 
        MissingETCuts(): dphicalc() {};
        bool selectEvent (EventData* event_data) const override;
    private:
        const DeltaPhi dphicalc; 
};

#endif