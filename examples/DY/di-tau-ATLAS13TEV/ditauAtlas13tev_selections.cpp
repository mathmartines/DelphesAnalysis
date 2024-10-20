#include "ditauAtlas13tev_selections.h"

void ElectronCandidates::selectObjects(EventData* event_data) const {
    /// clear electron vector
    event_data->electrons.clear();
    /// only electrons with |eta| < 2.47 and not with in 1.37 < |eta| < 1.52
    const TClonesArray* branch_electrons = event_data->getElectrons();
    for (int i = 0; i < branch_electrons->GetEntries(); i++) {
        Electron* electron = (Electron*) branch_electrons->At(i);
        if (abs(electron->Eta) < 1.37 || (abs(electron->Eta) > 1.52 && abs(electron->Eta) < 2.47))
            event_data->electrons.push_back(electron);
    }
}

void MuonCandidates::selectObjects(EventData* event_data) const {
    /// clear electron vector
    event_data->muons.clear();
    /// only electrons with |eta| < 2.5
    const TClonesArray* branch_muons = event_data->getMuons();
    for (int i = 0; i < branch_muons->GetEntries(); i++) {
        Muon* muon = (Muon*) branch_muons->At(i);
        if (abs(muon->Eta) < 2.5)
            event_data->muons.push_back(muon);
    }
}

void Jets::selectObjects(EventData* event_data) const {
    /// clear jet vector
    event_data->jets.clear();
    // pt < 20 and |eta| < 2.5
    const TClonesArray* branch_jets = event_data->getJets();
    for (int i = 0; i < branch_jets->GetEntries(); i++) {
        Jet* jet = (Jet*) branch_jets->At(i);
        if (jet->PT > 20 && abs(jet->Eta) < 2.5)
            event_data->jets.push_back(jet);
    }
}

void HadronicTaus::selectObjects(EventData* event_data) const {
    /// clear hadronic tau vector
    event_data->hadronic_taus.clear();
    /// loop over the selected jets
    for (Jet* jet: event_data->jets) {
        /// check if the jet has been tagged as a tau
        if (!jet->TauTag) continue; // if this does not help we can use the probabilities instead
        /// charge must be +1 or -1
        if (abs(jet->Charge) != 1) continue;
        /// the number of tracks has to be 1 or 3
        if (!(jet->NCharged == 1 || jet->NCharged == 3)) continue;
        /// removing eta window
        if(abs(jet->Eta) >= 1.37 && abs(jet->Eta) < 1.52) continue;
        /// all the tau candidates passed
        event_data->hadronic_taus.push_back(jet);
    }
}
