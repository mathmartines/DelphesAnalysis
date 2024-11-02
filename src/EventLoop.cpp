#include "DelphesAnalysis/EventLoop.h"

long double EventLoop::run(EventAnalysis* analysis, Distribution* distribution=nullptr) {
    /// adds all the root files to the chain
    for (auto file: files)
        chain.Add(file);
    
    /// creates the shared pointer to a tree reader object
    tree_reader = std::make_shared<ExRootTreeReader>(&chain);
    
    /// let EventData stores all the information needed for the analysis
    event_data->setTree(tree_reader);

    /// holds the number of events that passed the analysis
    int passed_evts = 0;
    
    /// launches the event loop
    for (int iEvent = 0; iEvent < tree_reader->GetEntries(); iEvent++) {
        /// gets the information about the current event
        tree_reader->ReadEntry(iEvent);
        /// delegates the analysis of the event to the analysis object
        if (analysis->processEvent(event_data)) passed_evts++; 
        /// updating the distributions if needed
        if (distribution) distribution->updateDistribution(event_data);
    }
    std::cout << "Passed: " << passed_evts << "/" << tree_reader->GetEntries() << std::endl;
    return static_cast<long double>(passed_evts) / tree_reader->GetEntries();
}

void EventLoop::reset() {
    /// clear the files list
    files.clear();
    /// resets the TChain
    chain.Reset();
}
    