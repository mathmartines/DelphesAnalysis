#include "SelectionsATLAS-ditau.h"


void ElectronCandidates::selectObjects(EventData* event_data) const {
    /// clear the electrons vector for the event
    EventData::ParticlesVector& electrons = event_data->getParticles("Electron");
    electrons.clear();
    /// only electrons with |eta| < 2.47 and not with in 1.37 < |eta| < 1.52
    const TClonesArray* branch_electrons = event_data->getBranch("Electron");
    for (int i = 0; i < branch_electrons->GetEntries(); i++) {
        Electron* electron = (Electron*) branch_electrons->At(i);
        if (abs(electron->Eta) < 1.37 || (abs(electron->Eta) > 1.52 && abs(electron->Eta) < 2.47))
            electrons.push_back(electron);
    }
}

void MuonCandidates::selectObjects(EventData* event_data) const {
    /// clear the muon vector for the analysis
    EventData::ParticlesVector& muons = event_data->getParticles("Muon");
    muons.clear();
    /// only electrons with |eta| < 2.5
    const TClonesArray* branch_muons = event_data->getBranch("Muon");
    for (int i = 0; i < branch_muons->GetEntries(); i++) {
        Muon* muon = (Muon*) branch_muons->At(i);
        if (abs(muon->Eta) < 2.5)
            muons.push_back(muon);
    }
}

void Jets::selectObjects(EventData* event_data) const {
    /// clear the jets vector for the analysis
    EventData::ParticlesVector& jets = event_data->getParticles("Jet");
    jets.clear();
    // pt < 20 and |eta| < 2.5
    const TClonesArray* branch_jets = event_data->getBranch("Jet");
    for (int i = 0; i < branch_jets->GetEntries(); i++) {
        Jet* jet = (Jet*) branch_jets->At(i);
        if (jet->PT > 20 && abs(jet->Eta) < 2.5)
            jets.push_back(jet);
    }
}

void HadronicTaus::selectObjects(EventData* event_data) const {
    /// clear the vector that stores the hadronic taus
    EventData::ParticlesVector& hadronic_taus = event_data->getParticles("HadronicTau");
    hadronic_taus.clear();

    /// we extract the hadronic taus from the jets
    EventData::ParticlesVector& jets = event_data->getParticles("Jet");
    for (auto& jet_: jets) {
        /// ptr to the location where Jet* is store
        Jet* jet = EventData::getPtrToParticle<Jet>(jet_);
        /// check if the jet has been tagged as a tau
        if (!jet->TauTag) continue;
        /// charge must be +1 or -1
        if (abs(jet->Charge) != 1) continue;
        /// the number of tracks has to be 1 or 3
        if (!countTracks(event_data, jet)) continue;
        /// removing eta window
        if(abs(jet->Eta) >= 1.37 && abs(jet->Eta) < 1.52) continue;
        /// add to the list of taus in the analysis
        hadronic_taus.push_back(jet);
    }
}

bool HadronicTaus::countTracks(EventData* event_data, Jet* hadronic_tau) const {
    /// holds the number of tracks around the jet
    int number_tracks = 0; 
    /// hadronic-tau four momentum
    const TLorentzVector& tau_momentum = hadronic_tau->P4();
    /// finds the tracks within a DeltaR < 0.2 of the jet
    const TClonesArray* branch_track = event_data->getBranch("Track");
    for (int i = 0; i < branch_track->GetEntries(); i++) {
        Track* track = (Track*) branch_track->At(i);
        // checks if the track is within a cone of 0.2 radius around the jet
        if (tau_momentum.DeltaR(track->P4()) < 0.2)
            number_tracks++;
    }
    return number_tracks == 1 || number_tracks == 3;
}

bool NumberOfLeptons::selectEvent(EventData* event_data) const {
    return event_data->getParticles("Electron").size() + event_data->getParticles("Muon").size() == number_of_lep;
}

bool NumberOfTauHad::selectEvent (EventData* event_data) const {
    return event_data->getParticles("HadronicTau").size() >= min_taus;
}

bool HadronicTausCut::selectEvent(EventData* event_data) const {
    selectTaus(event_data);
    /// events must have at least two hadronic taus 
    if (!number_of_taus_cut.selectEvent(event_data))
        return false;
    /// retriving the leading and subleading jets
    EventData::ParticlesVector& taus = event_data->getParticles("HadronicTau");
    Jet* leading_jet = EventData::getPtrToParticle<Jet>(taus[0]);
    /// leading jet pT cut
    if (leading_jet->PT < 130)
        return false;
    /// opposite charge check 
    if(!oppositeCharge(event_data))
        return false;
    Jet* subleading_jet = EventData::getPtrToParticle<Jet>(taus[1]);
    // back to back check
    return abs(obs_deltaphi.evaluateObservable({leading_jet->P4(), subleading_jet->P4()})) > 2.7;
}

void HadronicTausCut::selectTaus(EventData* event_data) const {
    /// current hadronic taus
    EventData::ParticlesVector& hadronic_taus = event_data->getParticles("HadronicTau");
    /// stores the right pt 
    std::vector<EventData::Particles> selected_taus;
    for(auto& tau_had: hadronic_taus) {
        Jet* tau_cand = EventData::getPtrToParticle<Jet>(tau_had);
        if(tau_cand->PT > 125)
                selected_taus.push_back(tau_had);
    }
    /// updating the list of hadronic taus in the event data
    hadronic_taus = selected_taus;
}

bool HadronicTausCut::oppositeCharge(EventData* event_data) const {
    /// finds the hadronic tau that has opposite charge of the leading tau
    Jet* subleading_tau = nullptr;
    EventData::ParticlesVector& hadronic_taus = event_data->getParticles("HadronicTau");
    Jet* leading_tau = EventData::getPtrToParticle<Jet>(hadronic_taus[0]);

    /// checking all the available candidates 
    for (int i = 1; i < hadronic_taus.size(); i++) {
        Jet* sublead_candidate = EventData::getPtrToParticle<Jet>(hadronic_taus[i]);
        if(leading_tau->Charge == -sublead_candidate->Charge) {
            subleading_tau = sublead_candidate;
            break;
        }
    }
    /// updates the list of hadronic taus
    if (subleading_tau) {
        hadronic_taus[1] = subleading_tau;
        return true;
    }
    return false;
}

bool bVeto::selectEvent(EventData* event_data) const {
    for (auto& jet_: event_data->getParticles("Jet")) {
        Jet* jet = EventData::getPtrToParticle<Jet>(jet_);
        if (jet->BTag)
            return false;
    }
    return true;
}

bool bTag::selectEvent(EventData* event_data) const {
    for (auto& jet_: event_data->getParticles("Jet")) {
        Jet* jet = EventData::getPtrToParticle<Jet>(jet_);
        if (jet->BTag)
            return true;
    }
    return false;
}

bool LeptonPtCut::selectEvent(EventData* event_data) const {
    if (event_data->getParticles("Electron").size() > 0) {
        Electron* electron = EventData::getPtrToParticle<Electron>(event_data->getParticles("Electron")[0]);
        return electron->PT > 30;
    }
    Muon* muon = EventData::getPtrToParticle<Muon>(event_data->getParticles("Muon")[0]);
    return muon->PT > 30;
}


bool TauLeptonEventsCuts::selectEvent(EventData* event_data) const {
    // lepton charge and momentum
    int lepton_charge;
    TLorentzVector lepton_momentum;

    /// saves the momentum and the charge of the lepton
    if (event_data->getParticles("Electron").size() == 1) {
        Electron* electron = EventData::getPtrToParticle<Electron>(event_data->getParticles("Electron")[0]);
        lepton_charge = electron->Charge;
        lepton_momentum = electron->P4();
    }
    else {
        Muon* muon = EventData::getPtrToParticle<Muon>(event_data->getParticles("Muon")[0]);
        lepton_charge = muon->Charge;
        lepton_momentum = muon->P4();
    }
    // checks if a hadronic tau with opposite charge is found
    if (!oppositeChargeHadTau(event_data, lepton_charge))
        return false;
    /// gets the hadronic tau
    Jet* tau = EventData::getPtrToParticle<Jet>(event_data->getParticles("HadronicTau")[0]);
    /// checking if they are back to back
    if (abs(delta_phi.evaluateObservable({lepton_momentum, tau->P4()})) <= 2.4)
        return false;
    /// getting the missing energy 
    const TClonesArray* branchMET = event_data->getBranch("MissingET");
    MissingET* met = (MissingET*) branchMET->At(0);
    TLorentzVector met_momentum;
    met_momentum.SetPtEtaPhiM(met->MET, met->Eta, met->Phi, 0);
    if (mtatlas.evaluateObservable({lepton_momentum, met_momentum}) >= 40)
        return false;
    /// invariant mass for lepton + tau
    double invmass = m.evaluateObservable({lepton_momentum, tau->P4()});
    /// removing Z mass window
    return invmass < 80 || invmass > 110;
}

bool TauLeptonEventsCuts::oppositeChargeHadTau(EventData* event_data, int lepton_charge) const {
    EventData::ParticlesVector& taus = event_data->getParticles("HadronicTau");
    for (auto& tau_ref: taus) {
        Jet* tau = EventData::getPtrToParticle<Jet>(tau_ref);
        if (tau->Charge == -lepton_charge && abs(tau->Eta) < 2.3) {
            /// updates the first entry to store the selected tau
            event_data->getParticles("HadronicTau")[0] = tau;
            return true;
        }
    }
    return false;
}
