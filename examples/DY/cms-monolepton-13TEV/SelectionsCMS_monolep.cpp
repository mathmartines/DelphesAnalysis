#include "SelectionsCMS_monolep.h"

void ElectronCandidates::selectObjects(EventData* event_data) const {
    /// acess the electrons branch
    const TClonesArray* electron_branch = event_data->getBranch("Electron");
    for (int iEle = 0; iEle < electron_branch->GetEntries(); iEle++) {
        Electron* electron = (Electron*) electron_branch->At(iEle);
        /// isolation criteria
        if (electron->SumPtNeutral >= 0.03 * electron->PT)
            continue;
        /// kinematical checks
        if (electron->PT > 35 && (abs(electron->Eta) < 1.4 || (abs(electron->Eta) < 1.57 && abs(electron->Eta) < 2.5))) 
            event_data->getParticles("Electron").push_back(electron);
    }
}

void MuonCandidates::selectObjects(EventData* event_data) const {
    /// acess the muons branch
    const TClonesArray* muon_branch = event_data->getBranch("Muon");
    for (int iMuon = 0; iMuon < muon_branch->GetEntries(); iMuon++) {
        Muon* muon = (Muon*) muon_branch->At(iMuon);
        /// isolation check
        if (muon->SumPtCharged >= 0.1 * muon->PT)
            continue;
        /// kinematical checks
        if (abs(muon->Eta) < 2.4)
            event_data->getParticles("Muon").push_back(muon);
    }
}

void Jets::selectObjects(EventData* event_data) const {
    /// acess the Jets branch
    const TClonesArray* jets_branch = event_data->getBranch("Jet");
    for (int iJet = 0; iJet < jets_branch->GetEntries(); iJet++) {
        Jet* jet = (Jet*) jets_branch->At(iJet);
        /// kinematical cuts
        if (jet->PT > 25 && abs(jet->Eta) < 2.5)
            event_data->getParticles("Jet").push_back(jet);
    }
}

bool NumberOfLeptons::selectEvent(EventData* event_data) const {
    return event_data->getParticles("Electron").size() + event_data->getParticles("Muon").size() >= 1;
}

bool ElectronEvent::selectEvent(EventData* event_data) const {
    EventData::ParticlesVector& electrons = event_data->getParticles("Electron");
    if (electrons.size() < 1)
        return false;
    Electron* leading = EventData::getPtrToParticle<Electron>(electrons[0]);
    return leading->PT > 200;
}

bool MuonEvent::selectEvent (EventData* event_data) const {
    EventData::ParticlesVector& muons = event_data->getParticles("Muon");
    if (muons.size() < 1)
        return false;
    Muon* muon = EventData::getPtrToParticle<Muon>(muons[0]);
    return muon->PT > 50;
}

bool AdditionalLeptonsVeto::selectEvent(EventData* event_data) const {
    /// assumes that the leading lepton for the analysis is the first entry depending if 
    /// we are looking to the electron or muon events
    int initialIndexEle = eleEvents? 1: 0;
    int initialIndexMuon = eleEvents? 0: 1;

    /// checking the electrons
    EventData::ParticlesVector& electrons = event_data->getParticles("Electron");
    for (int iEle = initialIndexEle; iEle < electrons.size(); iEle++) {
        Electron* electron = EventData::getPtrToParticle<Electron>(electrons[iEle]);
        if (electron->PT > 25)
            return false;
    }

    /// checking the muons 
    EventData::ParticlesVector& muons = event_data->getParticles("Muon");
    for (int iMuon = initialIndexMuon; iMuon < muons.size(); iMuon++) {
        Muon* muon = EventData::getPtrToParticle<Muon>(muons[iMuon]);
        if (muon->PT > 25)
            return false;
    }

    return true;
}

bool VetoOnJets::selectEvent(EventData* event_data) const {
    EventData::ParticlesVector& jets = event_data->getParticles("Jet");
    Muon* muon = EventData::getPtrToParticle<Muon>(event_data->getParticles("Muon")[0]);
    const TLorentzVector muon_momentum = muon->P4();

    /// holds the number of jets 
    int njets = 0;

    for (auto& jetvar: jets) {
        Jet* jet = EventData::getPtrToParticle<Jet>(jetvar);
        if (muon_momentum.DeltaR(jet->P4()) > 0.5)
            njets++;
    }

    return njets < 5;
}

bool bJetVeto::selectEvent(EventData* event_data) const {
    const TClonesArray* jets = event_data->getBranch("Jet");
    if (jets->GetEntries() == 0)
        return true;
    Jet* lead = (Jet*) jets->At(0);
    return !lead->BTag;
}

bool MissingETCuts::selectEvent(EventData* event_data) const {
    /// getting the missing energy 
    const TClonesArray* branchMET = event_data->getBranch("MissingET");
    MissingET* met = (MissingET*) branchMET->At(0);
    TLorentzVector met_momentum;
    met_momentum.SetPtEtaPhiM(met->MET, met->Eta, met->Phi, 0);
    /// getting the lepton momentum 
    TLorentzVector leptonMomentum;
    if (event_data->getParticles("Electron").size()) {
        Electron* ele = EventData::getPtrToParticle<Electron>(event_data->getParticles("Electron")[0]);
        leptonMomentum = ele->P4();
    } else {
        Muon* muon = EventData::getPtrToParticle<Muon>(event_data->getParticles("Muon")[0]);
        leptonMomentum = muon->P4();
    }
    double ptratio = leptonMomentum.Pt() / met->MET;
    return (ptratio > 0.4 && ptratio < 1.5) && abs(dphicalc.evaluateObservable({met_momentum, leptonMomentum})) > 2.5;
}