#include "DelphesAnalysis/EventAnalysis.h"

bool EventAnalysis::processEvent(EventData* event_data) const {
    /// resets all vectors in event_data
    event_data->resetVectors();
    /// apply all the selections
    obj_selection->selectObjects(event_data);
    /// checks if events passed all the cuts
    if (selection_cuts->selectEvent(event_data))
        return true;
    return false;
}