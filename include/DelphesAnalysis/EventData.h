#ifndef EVENTDATA_H
#define EVENTDATA_H

#include <iostream>
#include <vector>
#include <memory>
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "classes/DelphesClasses.h"

/**
 * @class - abstract class to store and access the data concerning the events
 */
class EventData {
    public:
        /// @brief - set the tree reader p  ointer to extract the data from
        virtual void setTree (std::shared_ptr<ExRootTreeReader> tree);
        
        /// @brief - acess to the branch information
        const TClonesArray* const &getJets() const {return branch_jets;};
        const TClonesArray* const &getElectrons() const {return branch_electrons;};
        const TClonesArray* const &getMuons() const {return branch_muons;};
        const TClonesArray* const &getMET() const {return branch_met;};
        const TClonesArray* const &getTracks() const {return branch_track;};

        /// @brief - vectors to store the pointers to the particles we'll use in the analysis
        std::vector<Electron*> electrons;
        std::vector<Muon*> muons;
        std::vector<Jet*> jets;
        std::vector<Jet*> hadronic_taus;

    protected:
        /// @brief - pointer to the tree object
        std::shared_ptr<ExRootTreeReader> tree_reader;

        /// default branches
        const TClonesArray* branch_jets = nullptr;
        const TClonesArray* branch_electrons = nullptr;
        const TClonesArray* branch_muons = nullptr;
        const TClonesArray* branch_met = nullptr;
        const TClonesArray* branch_track = nullptr;
};

#endif