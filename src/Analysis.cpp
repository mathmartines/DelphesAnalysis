#include "DelphesAnalysis/Analysis.h"

void Analysis::runAnalysis(TString filename) {
    /// make sure the event loop is ready
    event_loop.reset();

    /// making sure all references have been set in the event loop
    event_loop.setEventData(getEventData());
    event_loop.setObjectSelection(getObjectSelection());
    event_loop.setCuts(getCuts());

    /// adding the file
    event_loop.addFile(filename);

    /// initializing the loop
    event_loop.initialize();

    /// launch analysis
    event_loop.execute();
}