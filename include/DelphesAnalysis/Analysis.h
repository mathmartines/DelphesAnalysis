#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "TString.h"
#include "DelphesAnalysis/EventLoop.h"
#include "DelphesAnalysis/EventData.h"
#include "DelphesAnalysis/ObjectSelection.h"
#include "DelphesAnalysis/Cut.h"

/// @brief - Defines the framework to run a analysis
///          The specifics about the analysis: cuts, particle selections, and 
///          which data must be store need to be provide by the subclasses             
class Analysis {

    public:        
        /// @brief - returns a pointer to the object storing all the information about the event
        virtual EventData* getEventData() = 0;
        /// @brief - returns a pointer to the object that specifies how the particles must be selected for the analysis
        virtual const ObjectSelection* getObjectSelection() const = 0;
        /// @brief - returns a pointer to the cuts to be applied
        virtual const Cut* getCuts() const = 0;
        
        const TString getAnalysisName() const {return analysis_name;};

        /// @brief - runs the analysis
        void runAnalysis(TString filename);

    protected:
        /// @brief - runs the event loop
        EventLoop event_loop;
        /// @brief - name for the analysis
        TString analysis_name = "";
};

#endif