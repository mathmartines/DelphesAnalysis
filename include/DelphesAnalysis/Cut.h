#ifndef CUT_H
#define CUT_H

#include <iostream>
#include <vector>
#include "DelphesAnalysis/EventData.h"

/// @brief - Interface to hold the cut or cuts to be applied on the events.
class Cut {
    public: 
        /// @brief - applys the cut or cuts to decide wheter the event qualified as a signal
        /// @param event_data - pointer to the object that stores the information about the event
        /// @return - returns True if the event must be selected, and False otherwise
        virtual bool selectEvent(EventData* event_data) const = 0;
};

/// @brief - Stores and applies a list of all the cuts for a analysis
class AnalysisCuts: public Cut {
    public:
        AnalysisCuts (std::vector<const Cut*> cuts): cuts_(cuts) {};
        AnalysisCuts () = default;
        /// @brief - adds a new cut to the list of cuts
        void addCut(const Cut* cut) {cuts_.push_back(cut);};
        /// @brief - applies all the cuts. Returns True only if all cuts passed
        bool selectEvent(EventData* event_data) const override {
            for(auto cut: cuts_)
                if (!cut->selectEvent(event_data)) 
                    return false;
            return true;
        };

    private:
        std::vector<const Cut*> cuts_;
} ;

#endif 