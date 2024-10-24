#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "TString.h"
#include "DelphesAnalysis/EventData.h"
#include "DelphesAnalysis/ObjectSelection.h"
#include "DelphesAnalysis/Cut.h"

/// @brief - Defines the framework to run a analysis
///          The specifics about the analysis: cuts, particle selections, and 
///          which data must be store need to be provide by the subclasses             
class Analysis {

    public:
        /// @brief - applies all the selection cuts to the event
        ///          it returns true if the event passed all the cuts, and false otherwise
        bool processEvent(EventData* event_data) const;

        /// @brief - setters
        void setObjectSelection(const ObjectSelection* object_selection) {obj_selection = object_selection;};
        void setCuts(const Cut* cuts) {selection_cuts = cuts;};

    protected:
        /// @brief - tells how we need to select the objects (Leptons, Jets, etc) for the analysis
        const ObjectSelection* obj_selection = nullptr; 

        /// @brief - pointer to a Cut object which tells if the event must be selected or not
        const Cut* selection_cuts = nullptr;
};

#endif