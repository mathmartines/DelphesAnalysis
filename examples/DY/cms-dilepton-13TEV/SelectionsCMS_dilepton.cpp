#include "SelectionsCMS_dilepton.h"

void ElectronCandidatesCMS::selectObjects(EventData* event_data) const {
    /// clear the vector that stores the electrons
    event_data->getParticles("Electron").clear();
    /// acess the branch with the electrons
    const TClonesArray* branch_electrons = event_data->getBranch("Electron");
    for (int iEle = 0; iEle < branch_electrons->GetEntries(); iEle++) {
        Electron* candidate = (Electron*) branch_electrons->At(iEle);
        /// checks the kinematical cuts
        if (candidate->PT > 35 && (abs(candidate->Eta) < 1.44 || (abs(candidate->Eta) > 1.57 && abs(candidate->Eta) < 2.5)))
            event_data->getParticles("Electron").push_back(candidate);
    }
}

bool NumberOfElectrons::selectEvent(EventData* event_data) const {
    return event_data->getParticles("Electron").size() >= n;
}

void MuonCandidatesCMS::selectObjects(EventData* event_data) const {
    /// clear the vector that stores the muons
    // event_data->getParticles("Muon").clear();
    event_data->getParticles("Electron").clear();
    /// checks all the muons
    double pt = 0;
    // const TClonesArray* branch_muons = event_data->getBranch("Muon");
    const TClonesArray* branch_muons = event_data->getBranch("Electron");
    for (int iMuon = 0; iMuon < branch_muons->GetEntries(); iMuon++) {
        Electron* candidate = (Electron*) branch_muons->At(iMuon);
        /// kinematical cuts
        if (candidate->PT > 53 && abs(candidate->Eta) < 2.4)
            event_data->getParticles("Electron").push_back(candidate);
        // if (candidate->SumPtCharged < 0.1)
    }
}

bool MuonCandidatesCMS::trackPtSumCheck(EventData* event_data, Electron* muon) const {
    /// extracting the muon momentum
    const TLorentzVector muon_momentum = muon->P4();
    /// holds the track pt sum
    double ptsum = 0;
    /// branch with all the tracks
    const TClonesArray* tracks = event_data->getBranch("Track");
    for (int iTrack = 0; iTrack < tracks->GetEntries(); iTrack++) {
        Track* track = (Track*) tracks->At(iTrack);
        /// updates the sum only if the track is within 0.3 of the muon
        if (muon_momentum.DeltaR(track->P4()) < 0.3)
            ptsum += track->PT;
    }
    /// check if this is equivalent to the Isolation variable
    return (ptsum - muon->PT) < 0.1 * muon->PT;
}

bool NumberOfMuons::selectEvent(EventData* event_data) const {
    return event_data->getParticles("Electron").size() >= n;
}

bool InvariantMassCut::selectEvent(EventData* event_data) const {
    /// selecting the leading and subleading muons
    Electron* leading = EventData::getPtrToParticle<Electron>(event_data->getParticles("Electron")[0]);
    Electron* subleading = EventData::getPtrToParticle<Electron>(event_data->getParticles("Electron")[1]);
    /// calculates the invariant mass 
    return m.evaluateObservable({leading->P4(), subleading->P4()}) > 150;
}

bool OppositeChargeCut::selectEvent(EventData* event_data) const {
    /// selecting the leading and subleading muons
    Electron* leading = EventData::getPtrToParticle<Electron>(event_data->getParticles("Electron")[0]);
    Electron* subleading = EventData::getPtrToParticle<Electron>(event_data->getParticles("Electron")[1]);
    return leading->Charge == -subleading->Charge;
}

bool AngularDistanceCut::selectEvent(EventData* event_data) const { 
    /// selecting the leading and subleading muons
    const TLorentzVector lead_momentum = EventData::getPtrToParticle<Electron>(event_data->getParticles("Electron")[0])->P4();
    const TLorentzVector sublead_momentum = EventData::getPtrToParticle<Electron>(event_data->getParticles("Electron")[1])->P4();
    // evaluates the cosine of the angle
    double cosAngle = scalarProduct(lead_momentum, sublead_momentum) / (norm(lead_momentum) * norm(sublead_momentum));
    /// minimum allowed value
    double min_value = cos(M_PI - 0.02);
    return cosAngle > min_value;
}

double AngularDistanceCut::scalarProduct(const TLorentzVector& vec1, const TLorentzVector& vec2) const {
    return vec1.Px() * vec2.Px() + vec1.Py() * vec2.Py() + vec1.Pz() * vec2.Pz();
}

double AngularDistanceCut::norm(const TLorentzVector& vec) const {
    return sqrt(pow(vec.Px(), 2) + pow(vec.Py(), 2) + pow(vec.Pz(), 2));
}

