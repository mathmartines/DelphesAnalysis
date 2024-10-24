#ifndef OBJECT_SELECTION_H
#define OBJECT_SELECTION_H

#include <iostream>
#include <vector>
#include "DelphesAnalysis/EventData.h"

/// @brief - Interface for the particle and object selection for the analysis.
class ObjectSelection {
    public:
        /// @brief - Specify how the particles or event objects must be selected for the analysis
        /// @param event_data - stores the information about the event
        virtual void selectObjects(EventData* event_data) const = 0;
};

/// @brief - holds a set of particle selections
class CompositeSelections: public ObjectSelection {
    
    public:
        CompositeSelections() = default;
        CompositeSelections(std::vector<const ObjectSelection*> selections): selectors(selections) {};

        /// @brief - adds a new selection
        void addSelection(const ObjectSelection* selection) {selectors.push_back(selection);};

        /// @brief - to select the objects we need for the analysis we launch all the selectors
        void selectObjects(EventData* event_data) const override {
            for (auto selection: selectors) selection->selectObjects(event_data);
        };

    private:
        std::vector<const ObjectSelection*> selectors;
};


#endif