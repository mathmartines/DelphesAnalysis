#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <iostream>
#include <memory>
#include "TString.h"
#include "TChain.h"
#include "TClonesArray.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "DelphesAnalysis/EventData.h"
#include "DelphesAnalysis/ObjectSelection.h"
#include "DelphesAnalysis/Cut.h"


/**
 * @class - Responsible to iterate over all events store in a TChain.
 *          It performs the selection of the events that passed all the 
 *          selection cuts and builds the needed distributions.
*/
class EventLoop {

    public:
        /// @param filename - file path where the root file is stored
        EventLoop(): chain("Delphes"){};

        /// @brief - adds a new file to the analysis
        void addFile(TString filename) {files.push_back(filename);};

        /// @brief - setters
        void setEventData (EventData* evt_data) {event_data = evt_data;};
        void setObjectSelection (const ObjectSelection* obj_selection) {selection = obj_selection;};
        void setCuts (const Cut* cut) {selection_cuts = cut;};

        /// @brief - initialize all the variables for the execution of the analysis
        void initialize();

        /// @brief - runs the analysis to iterate over all the events in the file
        void execute();

        /// @brief - resets the TChain for a different run
        void reset();

    private:
        /// @brief - stores the chain to acess the files
        TChain chain;

        /// @brief - stores a pointer to the reader object to acess all the objects (Jets, electrons, etc).
        std::shared_ptr<ExRootTreeReader> tree_reader;

        /// @brief - vector to store all the files for the analysis
        std::vector<TString> files;

        /// @brief - pointer to the EventData object that which branches of data we must store
        EventData* event_data = nullptr;

        /// @brief - tells how we need to select the objects (Leptons, Jets, etc) for the analysis
        const ObjectSelection* selection = nullptr; 

        /// @brief - pointer to a Cut object which tells if the event must be selected or not
        const Cut* selection_cuts = nullptr;
};

#endif