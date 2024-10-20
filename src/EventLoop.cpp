#include "DelphesAnalysis/EventLoop.h"

void EventLoop::initialize(EventData* evt_data, const ObjectSelection* obj_selection, const Cut* cut) {
    /// adds all the root files to the chain
    for (auto file: files)
        chain.Add(file);
    /// creates the shared pointer to a tree reader object
    tree_reader = std::make_shared<ExRootTreeReader>(&chain);
    /// adds the tree to the data object
    event_data = evt_data;
    event_data->setTree(tree_reader);
    // sets the object selection and the cuts
    selection = obj_selection;
    selection_cuts = cut;
}

void EventLoop::execute() {
    // checks if the object has been initialized
    if (!tree_reader) 
        throw std::runtime_error("Calling execute while the event loop was not initialized.");
        
    // run analysis
    int passed_evts = 0;
    for (int index = 0; index < tree_reader->GetEntries(); index++) {
        if (index % 100 == 0)
            std::cout << "Reached " << index << " events" << std::endl;
        tree_reader->ReadEntry(index);
        // selects the objects for the analysis
        selection->selectObjects(event_data);
        // checks if the event passes all the cuts
        if (!selection_cuts->selectEvent(event_data)) continue;
        // updates the counter and the histograms
        passed_evts++;
    }
    std::cout << "Passed: " << passed_evts << "/" << tree_reader->GetEntries() << std::endl;
}