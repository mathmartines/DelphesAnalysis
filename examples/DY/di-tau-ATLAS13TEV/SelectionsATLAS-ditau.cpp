#include "SelectionsATLAS-ditau.h"

void ElectronCandidates::selectObjects(EventData* event_data) const {
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
    event_data->hadronic_taus.clear();
    /// loop over the selected jets
    for (Jet* jet: event_data->jets) {
        /// check if the jet has been tagged as a tau
        if (!jet->TauTag) continue; 
        /// charge must be +1 or -1
        if (abs(jet->Charge) != 1) continue;
        /// the number of tracks has to be 1 or 3
        if (!countTracks(event_data, jet)) continue;
        /// removing eta window
        if(abs(jet->Eta) >= 1.37 && abs(jet->Eta) < 1.52) continue;
        event_data->hadronic_taus.push_back(jet);
    }
}

bool HadronicTaus::countTracks(EventData* event_data, Jet* hadronic_tau) const {
    /// holds the number of tracks around the jet
    int number_tracks = 0; 
    /// hadronic-tau four momentum
    const TLorentzVector& tau_momentum = hadronic_tau->P4();
    /// finds the tracks within a DeltaR < 0.2 of the jet
    const TClonesArray* branch_track = event_data->getTracks();
    for (int i = 0; i < branch_track->GetEntries(); i++) {
        Track* track = (Track*) branch_track->At(i);
        // checks if the track is within a cone of 0.2 radius around the jet
        if (tau_momentum.DeltaR(track->P4()) < 0.2)
            number_tracks++;
    }
    return number_tracks == 1 || number_tracks == 3;
}

bool HadronicTausCut::selectEvent(EventData* event_data) const {
    /// stores the right pt
    std::vector<Jet*> selected_taus;
    for(Jet* tau_had: event_data->hadronic_taus)
        if(tau_had->PT > 125)
            selected_taus.push_back(tau_had);
    /// updating the list of hadronic taus in the event data
    event_data->hadronic_taus = selected_taus;
    /// events must have at least two hadronic taus 
    if (event_data->hadronic_taus.size() < 2)
        return false;
    // and the leading tau must have pT > 165
    if (event_data->hadronic_taus[0]->PT < 130)
        return false;
    /// opposite charge check 
    if(!oppositeCharge(event_data))
        return false;
    // back to back check
    return abs(obs_deltaphi.evaluateObservable({event_data->hadronic_taus[0]->P4(), event_data->hadronic_taus[1]->P4()})) > 2.7;
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

bool bVeto::selectEvent(EventData* event_data) const {
    for (Jet* jet: event_data->jets) {
        if (jet->BTag)
            return false;
    }
    return true;
}

bool bTag::selectEvent(EventData* event_data) const {
    for (Jet* jet: event_data->jets) {
        if (jet->BTag)
            return true;
    }
    return false;
}

bool SingleLeptonCut::selectEvent(EventData* event_data) const {
    int numberOfLeptons = event_data->electrons.size() + event_data->muons.size();
    return numberOfLeptons == 1;
}

bool LeptonPtCut::selectEvent(EventData* event_data) const {
    if (event_data->electrons.size() > 0)
        return event_data->electrons[0]->PT > 30;
    return event_data->muons[0]->PT > 30;
}


bool TauLeptonEventsCuts::selectEvent(EventData* event_data) const {
    // lepton charge and momentum
    int lepton_charge;
    TLorentzVector lepton_momentum;

    if (event_data->electrons.size() == 1) {
        lepton_charge = event_data->electrons[0]->Charge;
        lepton_momentum = event_data->electrons[0]->P4();
    }
    else {
        lepton_charge = event_data->muons[0]->Charge;
        lepton_momentum = event_data->muons[0]->P4();
    }
    // checks if a hadronic tau with opposite charge is found
    if (!oppositeChargeHadTau(event_data, lepton_charge))
        return false;
    /// checking if they are back to back
    if (abs(delta_phi.evaluateObservable({lepton_momentum, event_data->hadronic_taus[0]->P4()})) <= 2.4)
        return false;
    /// getting the missing energy 
    const TClonesArray* branchMET = event_data->getMET();
    MissingET* met = (MissingET*) branchMET->At(0);
    TLorentzVector met_momentum;
    met_momentum.SetPtEtaPhiM(met->MET, met->Eta, met->Phi, 0);
    if (mtatlas.evaluateObservable({lepton_momentum, met_momentum}) >= 40)
        return false;
    /// invariant mass for lepton + tau
    double invmass = m.evaluateObservable({lepton_momentum, event_data->hadronic_taus[0]->P4()});
    /// removing Z mass window
    return invmass < 80 || invmass > 110;
}

bool TauLeptonEventsCuts::oppositeChargeHadTau(EventData* event_data, int lepton_charge) const {
    for (Jet* had_tau: event_data->hadronic_taus) {
        if (had_tau->Charge == -lepton_charge && abs(had_tau->Eta) < 2.3) {
            /// updates the first to store the selected tau
            event_data->hadronic_taus[0] = had_tau;
            return true;
        }
    }
    return false;
}
