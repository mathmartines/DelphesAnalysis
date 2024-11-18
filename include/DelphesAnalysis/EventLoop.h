#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <iostream>
#include <memory>
#include "TString.h"
#include "TChain.h"
#include "TClonesArray.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "DelphesAnalysis/EventAnalysis.h"
#include "DelphesAnalysis/EventData.h"
#include "DelphesAnalysis/Distribution.h"

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

        /// @brief - defines which information must be extracted from the event
        void setEventData(EventData* evt_data) {event_data = evt_data;};

        /// @brief - runs the iteration over all the events in the file
        ///        - returns the efficiency
        int run(EventAnalysis* analysis, std::shared_ptr<Distribution> dist=nullptr);

        /// @brief - resets the TChain for a different run
        void reset();

    public: 
        /// @brief - stores the chain to acess the files
        TChain chain;

        /// @brief - stores a pointer to the reader object to acess all the objects (Jets, electrons, etc).
        std::shared_ptr<ExRootTreeReader> tree_reader;
        
        /// @brief - vector to store all the files for the analysis
        std::vector<TString> files;

        /// @brief - pointer to the EventData object that which branches of data we must store
        EventData* event_data = nullptr;
};

#endif