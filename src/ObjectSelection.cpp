#include "DelphesAnalysis/ObjectSelection.h"


void ElectronsOutBarrelEndcap::selectObjects(EventData* event_data) const {
    /// acess the electrons in the branch
    const TClonesArray* electrons = event_data->getBranch("Electron");
    for (int iEle = 0; iEle < electrons->GetEntries(); iEle++) {
        Electron* electron = (Electron*) electrons->At(iEle);
        if (electron->PT > ptmin_ && (abs(electron->Eta) < etamax_ && (abs(electron->Eta) > 1.52 || abs(electron->Eta)) < 1.37)) 
            event_data->getParticles("Electron").push_back(electron);
    }
}

void MuonCandidates::selectObjects(EventData* event_data) const {
    /// acess the muons in the branch
    const TClonesArray* muons = event_data->getBranch("Muon");
    for (int iMuon = 0; iMuon < muons->GetEntries(); iMuon++) {
        Muon* muon = (Muon*) muons->At(iMuon);
        if (muon->PT > ptmin_ && abs(muon->Eta) < etamax_)
            event_data->getParticles("Muon").push_back(muon);
    }
}

void JetCandidates::selectObjects(EventData* event_data) const {
    /// acess the jet branch
    const TClonesArray* jets = event_data->getBranch("Jet");
    for (int iJet = 0; iJet < jets->GetEntries(); iJet++) {
        Jet* jet = (Jet*) jets->At(iJet);
        if (jet->PT < ptmin_ && abs(jet->Eta) < etamax_)
            event_data->getParticles("Jet").push_back(jet);
    }
}