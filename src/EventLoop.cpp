#include "DelphesAnalysis/EventLoop.h"

int EventLoop::run(EventAnalysis* analysis, std::shared_ptr<Distribution> distribution) {
    /// adds all the root files to the chain
    for (auto file: files)
        chain.Add(file);
    
    /// creates the shared pointer to a tree reader object
    tree_reader = std::make_shared<ExRootTreeReader>(&chain);
    
    /// let EventData stores all the information needed for the analysis
    event_data->setTree(tree_reader);

    /// holds the number of events that passed the analysis
    int passed_evts = 0;
    /// total number of events
    int number_evts = tree_reader->GetEntries();
    
    /// launches the event loop
    for (int iEvent = 0; iEvent < number_evts; iEvent++) {
        if (iEvent % 10000 == 0)
            std::cout << "Reached event " << iEvent << std::endl;
        /// gets the information about the current event
        tree_reader->ReadEntry(iEvent);
        /// delegates the analysis of the event to the analysis object
        if (analysis->processEvent(event_data)) {
            passed_evts++; 
            /// updating the distributions if needed
            if (distribution) distribution->updateDistribution(event_data);
        }
    }
    if (distribution) distribution->displayNumberOfEvents();
    std::cout << "Passed: " << passed_evts << "/" << tree_reader->GetEntries() << std::endl;
    return number_evts;
}

void EventLoop::reset() {
    /// clear the files list
    files.clear();
    /// resets the TChain
    chain.Reset();
}
    