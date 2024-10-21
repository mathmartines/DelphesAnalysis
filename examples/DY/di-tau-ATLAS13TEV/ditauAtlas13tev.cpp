#include <iostream>
#include "DelphesAnalysis/EventLoop.h"
#include "DelphesAnalysis/EventData.h"
#include "DelphesAnalysis/Cut.h"
#include "ditauAtlas13tev_selections.h"

using namespace std;

int main() {
    /// specifies how to select the particles and objets for the analysis
    const ElectronCandidates electron_selection;
    const MuonCandidates muon_selection;
    const Jets jets_requirements;
    const HadronicTaus hadronic_taus_selection;
    CompositeSelections particle_selections ({&electron_selection, &muon_selection, &jets_requirements, &hadronic_taus_selection});

    /// selection cuts to be applied 
    const LeptonsVeto leptons_veto;
    const HadronicTausCut tau_had_cuts;
    const bVeto b_veto;
    AnalysisCuts ditau_had_bveto_analysis ({
        // &leptons_veto,
        &tau_had_cuts, 
        // &b_veto
    });

    /// stores the data for the event
    EventData data;
    /// pefforms the event loop and selects the signal events
    EventLoop evtLoop;
    /// sets all the needed pointers
    evtLoop.setEventData(&data);
    evtLoop.setObjectSelection(&particle_selections);
    evtLoop.setCuts(&ditau_had_bveto_analysis);
    /// adds the file that we must run the analysis on
    evtLoop.addFile("/Users/martines/Desktop/Physics/MG5_aMC_v2_9_20/test_tautau/Events/run_01/delphes2.root");
    // evtLoop.addFile("/home/martines/work/MG5_aMC_v3_1_1/test_tau/Events/run_02/delphes2.root");
    /// specifies the structure of the data and how the objects must be selected
    evtLoop.initialize();
    /// lanches the analysis
    evtLoop.execute();

    return 0;
}