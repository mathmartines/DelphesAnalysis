#include "SelectionsATLAS_dilepton.h"

void ElectronCandidatesATLAS::selectObjects(EventData* event_data) const {
    const TClonesArray* electrons_branch = event_data->getBranch("Electron");
    EventData::ParticlesVector& electrons  = event_data->getParticles("Electron");

    for (int iEle = 0; iEle < electrons_branch->GetEntries(); iEle++) {
        Electron* electron = (Electron*) electrons_branch->At(iEle);

        if (electron->PT > 30 && (abs(electron->Eta) < 1.37 || (abs(electron->Eta) > 1.52 && abs(electron->Eta) < 2.47)))
            electrons.push_back(electron);  
    }
}

void MuonCandidatesATLAS::selectObjects(EventData* event_data) const {
    const TClonesArray* muon_branch = event_data->getBranch("Muon");
    EventData::ParticlesVector& muons = event_data->getParticles("Muon");

    for (int iMuon = 0; iMuon < muon_branch->GetEntries(); iMuon++) {
        Muon* muon = (Muon*) muon_branch->At(iMuon);

        /// kinematical cuts
        if (muon->PT < 30 || abs(muon->Eta) > 2.5) continue;

        /// muon isolation check
        if (checkMuonIsolation(muon, event_data))
            muons.push_back(muon);
    }
}

bool MuonCandidatesATLAS::checkMuonIsolation(Muon* muon, EventData* event_data) const {
    const TLorentzVector& muon_fvector = muon->P4(); /// muon 4-vector
    const TClonesArray* tracks = event_data->getBranch("Track");
    double scalar_pt_sum = 0;
    double max_dR = 10. / muon->PT;

    for (int iTrack = 0; iTrack < tracks->GetEntries(); iTrack++) {
        Track* track = (Track*) tracks->At(iTrack);
        if (muon_fvector.DeltaR(track->P4()) < max_dR) 
            scalar_pt_sum += track->PT;
    }

    return (scalar_pt_sum - muon->PT) < 0.6 * muon->PT;
}

bool NumberOfElectrons::selectEvent (EventData* event_data) const {
    return event_data->getParticles("Electron").size() > 1;
}


bool NumberOfMuons::selectEvent (EventData* event_data) const {
    return event_data->getParticles("Muon").size() > 1;
}

bool  OppositeChargeMuons::selectEvent (EventData* event_data) const {
    /// leading and subleading muons
    const EventData::ParticlesVector& muons = event_data->getParticles("Muon");

    const Muon* lead = EventData::getPtrToParticle<Muon>(muons[0]);
    const Muon* sublead = EventData::getPtrToParticle<Muon>(muons[1]);

    return lead->Charge == -sublead->Charge;
}
