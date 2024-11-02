#ifndef OBSERVABLES_H
#define OBSERVABLES_H

#include <vector>
#include <math.h>
#include "TLorentzVector.h"


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

#endif