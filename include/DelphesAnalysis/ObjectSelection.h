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
class AnalysisSelections: public ObjectSelection {
    
    public:
        AnalysisSelections() = default;
        AnalysisSelections(std::vector<const ObjectSelection*> selections): selectors(selections) {};

        /// @brief - adds a new selection
        void addSelection(const ObjectSelection* selection) {selectors.push_back(selection);};

        /// @brief - to select the objects we need for the analysis we launch all the selectors
        void selectObjects(EventData* event_data) const override {
            for (auto selection: selectors) selection->selectObjects(event_data);
        };

    private:
        std::vector<const ObjectSelection*> selectors;
};

/** 
 *  @brief - Electron candidates
 *           Selects electrons with pT > ptmin and |eta| < etamax, 
 *           excluding the barrel-endcap transition region (1.37 < |eta| < 1.52)
 **/
class ElectronsOutBarrelEndcap: public ObjectSelection {
    public:
        ElectronsOutBarrelEndcap(double ptmin, double etamax): ptmin_(ptmin), etamax_(etamax) {};
        void selectObjects(EventData* event_data) const override;
    private:
        double ptmin_, etamax_;
};

/**
 * @brief - Muon candidates.
 *          Selects muons with pt > ptmin and eta < etamax
 */
class MuonCandidates: public ObjectSelection {
    public:
        MuonCandidates(double ptmin, double etamax): etamax_(etamax), ptmin_(ptmin) {};
        void selectObjects (EventData* event_data) const override;
    private:
        double etamax_, ptmin_;
};

/**
 * @brief - Jet selection
 *          Selects jets with pt > ptmin and |eta| < etamax
 */
class JetCandidates: public ObjectSelection {
    public:
        JetCandidates(double ptmin, double etamax): etamax_(etamax), ptmin_(ptmin) {};
        void selectObjects (EventData* event_data) const override;
    private:
        double etamax_, ptmin_;
};

#endif