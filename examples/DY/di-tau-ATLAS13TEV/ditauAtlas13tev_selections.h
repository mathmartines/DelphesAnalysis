#ifndef ATLAS13TEVDITAU_SELECTIONS_H
#define ATLAS13TEVDITAU_SELECTIONS_H

#include <iostream>
#include <vector>
#include "TClonesArray.h"
#include "DelphesAnalysis/ObjectSelection.h"

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
};

#endif