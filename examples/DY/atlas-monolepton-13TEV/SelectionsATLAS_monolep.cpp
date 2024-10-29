#include "SelectionsATLAS_monolep.h"

void JetSelectionsATLAS_monolep::selectObjects(EventData* event_data) const {
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
    TLorentzVector met_momentum;
    met_momentum.SetPtEtaPhiM(met->MET, met->Eta, met->Phi, 0);
    /// getting the lepton momentum
    TLorentzVector leptonmom;
    if (electronevt) {
        Electron* ele = EventData::getPtrToParticle<Electron>(event_data->getParticles("Electron")[0]);
        leptonmom = ele->P4();
    } else {
        Muon* muon = EventData::getPtrToParticle<Muon>(event_data->getParticles("Muon")[0]);
        leptonmom = muon->P4();
    }
    return mtcalc.evaluateObservable({leptonmom, met_momentum}) > mtmin_;
}