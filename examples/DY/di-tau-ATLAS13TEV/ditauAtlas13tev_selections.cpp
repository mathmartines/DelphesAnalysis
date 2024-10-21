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
        // std::cout << jet->BTag << std::endl;
        // if (!jet->TauTag) continue; 
        /// charge must be +1 or -1
        // if (abs(jet->Charge) != 1) continue;
        // std::cout << "pT: " << jet->PT << std::endl;
        // std::cout << "tau-tag: " << jet->TauTag << std::endl;
        // std::cout << "tau-weight: " << jet->TauWeight << std::endl;
        // std::cout << "tau-NCharged: " << jet->NCharged << std::endl;
        /// the number of tracks has to be 1 or 3
        // if (!(jet->NCharged == 1 || jet->NCharged == 3)) continue;
        /// removing eta window
        if(abs(jet->Eta) >= 1.37 && abs(jet->Eta) < 1.52) continue;
        /// all the tau candidates passed
        event_data->hadronic_taus.push_back(jet);
    }
}

bool HadronicTausCut::selectEvent(EventData* event_data) const {
    /// stores the right pt
    std::vector<Jet*> selected_taus;
    for(Jet* tau_had: event_data->hadronic_taus)
        if(tau_had->PT > 160)
            selected_taus.push_back(tau_had);
    /// updating the list of hadronic taus in the event data
    event_data->hadronic_taus = selected_taus;
    /// events must have at least two hadronic taus 
    if (event_data->hadronic_taus.size() < 2)
        return false;
    // and the leading tau must have pT > 165
    if (event_data->hadronic_taus[0]->PT < 165)
        return false;
    /// opposite charge check 
    if(!oppositeCharge(event_data))
        return false;
    // back to back check
    return backToBack(event_data);
}

bool HadronicTausCut::oppositeCharge(EventData* event_data) const {
    /// finds the hadronic tau that has opposite charge of the leading tau
    Jet* subleading_tau = nullptr;
    for (int i = 1; i < event_data->hadronic_taus.size(); i++) {
        if(event_data->hadronic_taus[0]->Charge == -event_data->hadronic_taus[i]->Charge) {
            subleading_tau = event_data->hadronic_taus[i];
            break;
        }
    }
    /// updates the list of hadronic taus
    if (subleading_tau) {
        event_data->hadronic_taus[1] = subleading_tau;
        return true;
    }
    return false;
}

bool HadronicTausCut::backToBack(EventData* event_data) const {
    /// azimuthal diff
    double dphi = event_data->hadronic_taus[0]->Phi - event_data->hadronic_taus[1]->Phi;
    if (dphi > M_PI) 
        dphi -= 2 * M_PI;
    else if(dphi < -M_PI)
        dphi += 2 * M_PI;
    return abs(dphi) > 2.7;
}

bool bVeto::selectEvent(EventData* event_data) const {
    for (Jet* jet: event_data->jets) {
        if (jet->BTag)
            return false;
    }
    return true;
}