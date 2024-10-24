#include "DelphesAnalysis/Analysis.h"

bool Analysis::processEvent(EventData* event_data) const {
    /// apply all the selections
    obj_selection->selectObjects(event_data);
    /// checks if events passed all the cuts
    if (selection_cuts->selectEvent(event_data))
        return true;
    return false;
}