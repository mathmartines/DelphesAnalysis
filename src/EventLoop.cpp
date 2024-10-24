#include "DelphesAnalysis/EventLoop.h"

void EventLoop::initialize() {
    /// adds all the root files to the chain
    for (auto file: files)
        chain.Add(file);
    /// creates the shared pointer to a tree reader object
    tree_reader = std::make_shared<ExRootTreeReader>(&chain);
    /// checks if all the pointers have been set
    if (!event_data || !selection || !selection_cuts)
        throw std::runtime_error("Calling initialize with references to null pointers.");
    /// adds the tree to the data object
    event_data->setTree(tree_reader);
}

void EventLoop::execute() {
    // checks if the object has been initialized
    if (!tree_reader) 
        throw std::runtime_error("Calling execute while the event loop was not initialized.");
    // run analysis
    int passed_evts = 0;
    for (int index = 0; index < tree_reader->GetEntries(); index++) {
        // if (index % 1000 == 0)
        //     std::cout << "Reached " << index << " events" << std::endl;
        tree_reader->ReadEntry(index);         
        // selects the objects for the analysis
        selection->selectObjects(event_data);
        // std::cout << event_data->hadronic_taus.size() << std::endl;
        // checks if the event passes all the cuts
        if (!selection_cuts->selectEvent(event_data)) continue;
        // updates the counter and the histograms
        passed_evts++;
    }
    std::cout << "Passed: " << passed_evts << "/" << tree_reader->GetEntries() << std::endl;
}

void EventLoop::reset() {
    /// clear the files list
    files.clear();
    /// resets the TChain
    chain.Reset();
}