#ifndef SELECTIONS_ATLAS_MONOLEP_H
#define SELECTIONS_ATLAS_MONOLEP_H

#include "TLorentzVector.h"
#include "DelphesAnalysis/EventData.h"
#include "DelphesAnalysis/ObjectSelection.h"
#include "DelphesAnalysis/Cut.h"
#include "DelphesAnalysis/Observable.h"

/**
 * @brief - Selects jets 
 *          pT > 20 GeV if |eta| < 2.4
 *          pT > 30 GeV if |eta| >= 2.4
 */
class JetSelectionsATLAS_monolep: public ObjectSelection {
    public:
        void selectObjects(EventData* event_data) const override;
};

/**
 * @brief - the leading electron must have pt > 65 GeV
 */
class LeadingElectronCut: public Cut {
    public:
        bool selectEvent (EventData* event_data) const override;
};

/**
 * @brief - the leading muon must have pt > 55 GeV
 */
class LeadingMuonCut: public Cut {
    public:
        bool selectEvent (EventData* event_data) const override;
};

/**
 * @brief - for the electron channel, we must veto events where we have an 
 *          additional lepton with pT > 20 GeV
 */
class AddLeptonVetoElectronChannel: public Cut {
    public:
        bool selectEvent (EventData* event_data) const override;
};

/**
 * @brief - for the electron channel, we must veto events where we have an 
 *          additional lepton with pT > 20 GeV if Delta R (l, mu) > 0.1
 */
class AddLeptonVetoMuonChannel: public Cut {
    public:
        bool selectEvent (EventData* event_data) const override;
};

/**
 * @brief - cut on the missing energy
 */
class MissingETCut: public Cut {
    public:
        MissingETCut(double metmin): metmin_(metmin) {};
        bool selectEvent (EventData* event_data) const override;
    private:
        double metmin_;
};

/**
 * @brief - cut on the transverse mass
 */
class TransverseMassCut: public Cut {
    public:
        TransverseMassCut(double mtmin, bool eleEvt): mtmin_(mtmin), mtcalc(), electronevt(eleEvt) {};
        bool selectEvent (EventData* event_data) const override;
    private:
        double mtmin_;
        bool electronevt;
        const TransverseMassATLAS mtcalc;
};

#endif