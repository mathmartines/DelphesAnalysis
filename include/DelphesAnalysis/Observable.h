#ifndef OBSERVABLES_H
#define OBSERVABLES_H

#include <vector>
#include <math.h>
#include "TLorentzVector.h"
#include "TString.h"
#include "classes/DelphesClasses.h"
#include "DelphesAnalysis/EventData.h"


class Observable {
    public:
        Observable() = default;
        virtual double evaluateObservable(const std::vector<TLorentzVector>& momentums) const = 0;
};

/// @brief - evaluates the angular difference in the transverse plane
///          the result is with in the interval -pi and pi
class DeltaPhi: public Observable {
    public:
        double evaluateObservable(const std::vector<TLorentzVector>& momentums) const override;
};

/// @brief - evaluates the transverse mass using all the momentums
class TransverseMass: public Observable {
    public:
        double evaluateObservable(const std::vector<TLorentzVector>& momentums) const override;
};

/// @brief - evaluates the invariant mass using all the momentums
class InvariantMass: public Observable {
    public:
        double evaluateObservable(const std::vector<TLorentzVector>& momentums) const override;
};

/// @brief - Evaluates the transverse mass using the expression given by the ATLAS paper XXXX.XXX
class TransverseMassATLAS: public Observable {
    public: 
        TransverseMassATLAS(): dphi(){};
        double evaluateObservable(const std::vector<TLorentzVector>& momentums) const override;
    private:
        const DeltaPhi dphi;
};

/// @brief - Evaluates a observable for a given event
class EventObservable {
    public:
        virtual double evaluateObservable(const EventData* event_data) const = 0;
};

/// @brief - Evaluates the invariant mass for a pair of leptons
class DielectronInvariantMass: public EventObservable {
    public:
        double evaluateObservable(const EventData* event_data) const override;

};

template <typename T>
class LeptonInvariantMass: public EventObservable {
    public:
        LeptonInvariantMass(TString lepton_flavor): lepton_flavor_(lepton_flavor) {};
        double evaluateObservable(const EventData* event_data) const override;
    private:
        TString lepton_flavor_;
};


template <typename T>
/// @brief - Evaluates the transvere mass for the monolepton events
class TransverseMassEventObs: public EventObservable {
    public:
        TransverseMassEventObs(TString lepton_flavor): lepton_flavor_(lepton_flavor) {};
        double evaluateObservable(const EventData* event_data) const override;

    private:
        /// Name of the vector that holds the lepton candidate
        TString lepton_flavor_;
};

template<typename T>
double TransverseMassEventObs<T>::evaluateObservable(const EventData* event_data) const {
    /// getting the lepton 
    T* electron = EventData::getPtrToParticle<T>(event_data->getParticles(lepton_flavor_)[0]);
    /// get the missing ET
    const TClonesArray* branchMET = event_data->getBranch("MissingET");
    MissingET* met = (MissingET*) branchMET->At(0);
    /// calculating the angular diff
    double delta_phi = electron->Phi - met->Phi;
    // invariant mass
    return sqrt(2 * electron->PT * met->MET * (1 - cos(delta_phi)));
}

template<typename T>
double LeptonInvariantMass<T>::evaluateObservable(const EventData* event_data) const {
    /// acess the electrons in the event
    EventData::ParticlesVector leptons = event_data->getParticles(lepton_flavor_);
    /// selected electrons
    T* lepton1 = EventData::getPtrToParticle<T>(leptons[0]);
    T* lepton2 = EventData::getPtrToParticle<T>(leptons[1]);
    /// returns the invariant mass
    return (lepton1->P4() + lepton2->P4()).M();
}

#endif