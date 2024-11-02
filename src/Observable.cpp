#include "DelphesAnalysis/Observable.h"


double DeltaPhi::evaluateObservable(const std::vector<TLorentzVector>& momentums) const {
    double dphi = momentums[0].Phi() - momentums[1].Phi();
    if (dphi > M_PI) 
        dphi -= 2 * M_PI;
    else if(dphi < -M_PI)
        dphi += 2 * M_PI;
    return dphi;
}

double TransverseMass::evaluateObservable(const std::vector<TLorentzVector>& momentums) const {
    TLorentzVector total_momentum;
    total_momentum.SetPtEtaPhiE(0, 0, 0, 0);
    for (auto momentum: momentums)
        total_momentum += momentum;
    return total_momentum.Mt();
}

double InvariantMass::evaluateObservable(const std::vector<TLorentzVector>& momentums) const {
    TLorentzVector total_momentum;
    total_momentum.SetPtEtaPhiE(0, 0, 0, 0);
    for (auto momentum: momentums)
        total_momentum += momentum;
    return total_momentum.M();
}

double TransverseMassATLAS::evaluateObservable(const std::vector<TLorentzVector>& momentums) const {
    double delta_phi = dphi.evaluateObservable(momentums);
    return sqrt(2 * momentums[0].Pt() * momentums[1].Pt() * (1 - cos(delta_phi)));
}

double DielectronInvariantMass::evaluateObservable(const EventData* event_data) const {
    /// acess the electrons in the event
    EventData::ParticlesVector electrons = event_data->getParticles("Electron");
    /// selected electrons
    Electron* electron1 = EventData::getPtrToParticle<Electron>(electrons[0]);
    Electron* electron2 = EventData::getPtrToParticle<Electron>(electrons[1]);
    /// returns the invariant mass
    return (electron1->P4() + electron2->P4()).M();
}
