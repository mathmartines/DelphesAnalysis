#ifndef SELECTIONS_CMS_DILEPTON_H
#define SELECTIONS_CMS_DILEPTON_H

#include <math.h>
#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "classes/DelphesClasses.h"
#include "DelphesAnalysis/ObjectSelection.h"
#include "DelphesAnalysis/Cut.h"
#include "DelphesAnalysis/Observable.h"
#include "DelphesAnalysis/EventData.h"

/// @brief - Selects the electron candidates for the analysis
///        - pT > 35 GeV and |eta| < 1.44 or 1.57 < |eta| < 2.5
class ElectronCandidatesCMS: public ObjectSelection {
    public:
        void selectObjects(EventData* event_data) const override;
};

/// @brief - Selects the muon candidates for the analysis
///        - pT > 53 GeV and |eta| < 2.4
///        - The pt sum of all the tracks around a cone of Delta R = 0.3 has to be less than 10% of the muon momentum
class MuonCandidatesCMS: public ObjectSelection {
    public:
        void selectObjects(EventData* event_data) const override;
    private:
        /// @brief - checks if the pt sum of the track is less than 10% of the muon momentum 
        bool trackPtSumCheck(EventData* event_data, Electron* muon) const;
};

/// @brief - Selects events only with the number of electrons >= n
class NumberOfElectrons: public Cut {
    public:
        NumberOfElectrons(int nelectrons): n(nelectrons) {};
        bool selectEvent (EventData* event_data) const override;
    private:
        /// @brief - stores the minimum number of electrons required in the event
        int n;
};

/// @brief - Selects events only with the number of muons >= n
class NumberOfMuons: public Cut {
    public:
        NumberOfMuons(int nmuons): n(nmuons) {};
        bool selectEvent (EventData* event_data) const override;
    private:
        /// @brief - stores the minimum number of muons required in the event
        int n;
};

/// @brief - Invariant mass cut on the muon pair m > 150 GeV
class InvariantMassCut: public Cut {
    public:
        InvariantMassCut(): m(){};
        bool selectEvent (EventData* event_data) const override;
    private:
        const InvariantMass m;
};

/// @brief - Muons must have opposite electric charge
class OppositeChargeCut: public Cut {
    public: 
        bool selectEvent (EventData* event_data) const override;
};

/// @brief - 3D angular distance of the muons must be less than pi - 0.02
class AngularDistanceCut: public Cut {
    public:
        bool selectEvent (EventData* event_data) const override;
    
    private:
        double scalarProduct(const TLorentzVector& vec1, const TLorentzVector& vec2) const;
        double norm (const TLorentzVector& vec) const;
};

#endif