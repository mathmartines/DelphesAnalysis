#ifndef SELECTIONS_ATLAS_DILEPTON_H
#define SELECTIONS_ATLAS_DILEPTON_H

#include "TLorentzVector.h"
#include "DelphesAnalysis/EventData.h"
#include "DelphesAnalysis/ObjectSelection.h"
#include "DelphesAnalysis/Cut.h"
#include "DelphesAnalysis/Observable.h"

/// @brief - pT > 30 GeV and |eta| < 2.47 and not in 1.37 < |eta| < 1.52
class ElectronCandidatesATLAS: public ObjectSelection {
    public:
        void selectObjects(EventData* event_data) const override;
};

/// @brief - pT > 30 GeV and |eta| < 2.5
///        - muon isolation: scalar sum of the tracks of around the muon direction,
///          exluding the muon itself, must be less than 6% of the muon pT.
///          The cone radius varies with the muon pT
class MuonCandidatesATLAS: public ObjectSelection {
    public:
        void selectObjects(EventData* event_data) const override;
    private:
        bool checkMuonIsolation(Muon* muon, EventData* event_data) const;
};

/// @brief - We need to have at least two electrons in the event
class NumberOfElectrons: public Cut {
    public:
        bool selectEvent (EventData* event_data) const override;
};

/// @brief - At least two muons in the event
class NumberOfMuons: public Cut {
    public: 
        bool selectEvent (EventData* event_data) const override;
};

/// @brief - The muon pair must have opposite charge
class OppositeChargeMuons: public Cut {
    public:
        bool selectEvent (EventData* event_data) const override;
};

#endif