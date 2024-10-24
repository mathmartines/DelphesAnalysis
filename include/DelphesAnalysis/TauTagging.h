#ifndef TAU_TAGGING_H
#define TAU_TAGGING_H

#include "classes/DelphesClasses.h"
#include "DelphesAnalysis/EventData.h"


class TauTagging {

    public:
        /// @brief - runs the tau tagging algorithm on the Jet 
        bool run (Jet* tau_candidate, EventData* event_data) const;
    
    private:
        // EfficiencyFormula* efficiency_formula;
};

#endif