#include "SelectionsATLAS_monolep.h"

void ElectronCandidatesATLAS::selectObjects (EventData* event_data) const {
    /// Acess the electrons branch
    const TClonesArray* branch_electrons = event_data->getBranch("Electron");
    
    /// Selects all electrons passing the cuts
    for (int iEle = 0; iEle < branch_electrons->GetEntries(); iEle++) {
        Electron* electron = (Electron*) branch_electrons->At(iEle);
        
        /// Checks the pT
        if (electron->PT < 20) 
            continue;

        /// Checks the pseudo-rapidity
        double abs_eta = abs(electron->Eta);
        if (abs_eta > 2.47 || (abs_eta > 1.37 && abs_eta < 1.52)) 
            continue;
                
        /// Passed all the cuts - adds to the list of electron candidates
        event_data->getParticles("Electron").push_back(electron);
    }
}

void MuonCandidatesATLAS::selectObjects (EventData* event_data) const {
    /// Acess the muon branch
    const TClonesArray* branch_muons = event_data->getBranch("Muon");

    /// Selects the Muon candidates
    for(int iMuon = 0; iMuon < branch_muons->GetEntries(); iMuon++) {
        Muon* muon = (Muon*) branch_muons->At(iMuon);

        /// Checks the pT
        if (!(muon->PT > 20 && abs(muon->Eta) < 2.5))
            continue;
            
        /// Check Muon isolation
        if (!checkIsolation(muon, event_data))
            continue;
        
        /// Adds to the list of muon candidates
        event_data->getParticles("Muon").push_back(muon);
    }
}

bool MuonCandidatesATLAS::checkIsolation(const Muon* muon, const EventData* event_data) const {
    /// Calculates the radiues of the isolation cone
    double dR = 10./ muon->PT < 0.3 ? 10/muon->PT : 0.3;

    /// Acess the tracks
    const TClonesArray* tracks = event_data->getBranch("Track");

    /// Holds the sum of the pT of all the tracks around the muon
    double ptsum = 0;

    /// Checks all the tracks
    for (int iTrack = 0; iTrack < tracks->GetEntries(); iTrack++) {
        Track* track = (Track*) tracks->At(iTrack);
        /// checks the minimum pT and distance from the muon
        /// also ensures that the track does not represent the track that is associated with the muon
        if (track->PT > 1 && muon->P4().DeltaR(track->P4()) < dR && track->Particle != muon->Particle) 
            ptsum += track->PT;
    }

    /// Isolation
    return ptsum < 0.15 * muon->PT;
}

void JetCandidatesATLAS::selectObjects(EventData* event_data) const {
    const TClonesArray* jets = event_data->getBranch("Jet");
    for (int iJet = 0; iJet < jets->GetEntries(); iJet++) {
        Jet* jet = (Jet*) jets->At(iJet);
        double ptmax = abs(jet->Eta) <= 2.4? 20: 30;
        if (jet->PT > ptmax)
            event_data->getParticles("Jet").push_back(jet);
    }
}

bool LeadingElectronCut::selectEvent(EventData* event_data) const {
    EventData::ParticlesVector& electrons = event_data->getParticles("Electron");
    if (electrons.size() == 0)
        return false;
    Electron* leading = EventData::getPtrToParticle<Electron>(electrons[0]);
    return leading->PT > 65;
}

bool LeadingMuonCut::selectEvent(EventData* event_data) const {
    EventData::ParticlesVector& muons = event_data->getParticles("Muon");
    if (muons.size() == 0)
        return false;
    Muon* leading = EventData::getPtrToParticle<Muon>(muons[0]);
    return leading->PT > 55;
}

bool AddLeptonVetoElectronChannel::selectEvent(EventData* event_data) const {
    EventData::ParticlesVector electrons = event_data->getParticles("Electron");
    EventData::ParticlesVector muons = event_data->getParticles("Muon");

    /// checking the electrons 
    if (electrons.size() > 1) {
        EventData::ParticlesVector::iterator itEle = electrons.begin() + 1;
        for (; itEle != electrons.end(); itEle++) {
            Electron* electron = EventData::getPtrToParticle<Electron>(*itEle);
            if (electron->PT > 20)
                return false;
        }
    }

    /// checking the muons
    if (muons.size() > 0) {
        EventData::ParticlesVector::iterator itMuon = muons.begin();
        for (; itMuon != muons.end(); itMuon++) {
            Muon* muon = EventData::getPtrToParticle<Muon>(*itMuon);
            if (muon->PT > 20)
                return false;
        }
    }

    return true;
}

bool AddLeptonVetoMuonChannel::selectEvent(EventData* event_data) const {
    EventData::ParticlesVector electrons = event_data->getParticles("Electron");
    EventData::ParticlesVector muons = event_data->getParticles("Muon");

    // leading muon momentum 
    Muon* leading = EventData::getPtrToParticle<Muon>(muons[0]);
    const TLorentzVector muon_momentum = leading->P4();

    /// checking the electrons 
    if (electrons.size() > 0) {
        EventData::ParticlesVector::iterator itEle = electrons.begin() ;
        for (; itEle != electrons.end(); itEle++) {
            Electron* electron = EventData::getPtrToParticle<Electron>(*itEle);
            if (muon_momentum.DeltaR(electron->P4()) > 0.1 && electron->PT > 20)
                return false;
        }
    }

    /// checking the muons
    if (muons.size() > 1) {
        EventData::ParticlesVector::iterator itMuon = muons.begin() + 1;
        for (; itMuon != muons.end(); itMuon++) {
            Muon* muon = EventData::getPtrToParticle<Muon>(*itMuon);
            if (muon->PT > 20)
                return false;
        }
    }

    return true;
}

bool MissingETCut::selectEvent(EventData* event_data) const {
    const TClonesArray* missinget = event_data->getBranch("MissingET");
    MissingET* met = (MissingET*) missinget->At(0);
    return met->MET > metmin_;
}

bool TransverseMassCut::selectEvent(EventData* event_data) const {
    /// getting the missing energy 
    const TClonesArray* branchMET = event_data->getBranch("MissingET");
    MissingET* met = (MissingET*) branchMET->At(0);

    /// getting the lepton momentum
    TLorentzVector leptonmom;
    if (electronevt) {
        Electron* ele = EventData::getPtrToParticle<Electron>(event_data->getParticles("Electron")[0]);
        leptonmom = ele->P4();
    } else {
        Muon* muon = EventData::getPtrToParticle<Muon>(event_data->getParticles("Muon")[0]);
        leptonmom = muon->P4();
    }

    /// Calculates the transverse mass
    double mt = sqrt(2 * leptonmom.Pt() * met->MET * (1 - cos(met->Phi - leptonmom.Phi())));

    /// Applies the cut
    return mt > mtmin_;
}