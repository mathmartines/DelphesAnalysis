#include "ditauAtlas13tev_cuts.h"

bool HadronicTausCut::selectEvent(EventData* event_data) const {
    /// stores the right pt
    std::vector<Jet*> selected_taus;
    for(Jet* tau_had: event_data->hadronic_taus)
        if(tau_had->PT > 160)
            selected_taus.push_back(tau_had);
    /// updating the list of hadronic taus in the event data
    event_data->hadronic_taus = selected_taus;
    /// events must have at least two hadronic taus 
    if (event_data->hadronic_taus.size() < 2)
        return false;
    // and the leading tau must have pT > 165
    if (event_data->hadronic_taus[0]->PT < 165)
        return false;
    /// opposite charge check 
    return oppositeCharge(event_data);
}

bool HadronicTausCut::oppositeCharge(EventData* event_data) const {
    /// finds the hadronic tau that has opposite charge of the leading tau
    Jet* subleading_tau = nullptr;
    for (int i = 1; i < event_data->hadronic_taus.size(); i++) {
        if(event_data->hadronic_taus[0]->Charge == -event_data->hadronic_taus[i]->Charge) {
            subleading_tau = event_data->hadronic_taus[i];
            break;
        }
    }
    /// updates the list of hadronic taus
    if (subleading_tau) {
        event_data->hadronic_taus[1] = subleading_tau;
        return true;
    }
    return false;
}

bool bVeto::selectEvent(EventData* event_data) const {
    for (Jet* jet: event_data->jets) {
        if (jet->BTag)
            return false;
    }
    return true;
}