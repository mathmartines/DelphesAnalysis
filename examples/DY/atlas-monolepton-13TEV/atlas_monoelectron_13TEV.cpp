#include <iostream>
#include <vector>
#include "TString.h"
#include "DelphesAnalysis/ObjectSelection.h"
#include "DelphesAnalysis/EventLoop.h"
#include "DelphesAnalysis/EventData.h"
#include "DelphesAnalysis/EventAnalysis.h"
#include "EventDataATLAS_monolep.h"
#include "SelectionsATLAS_monolep.h"

using namespace std;

int main() {
    /// particle selections for the ATLAS analysis
    const ElectronsOutBarrelEndcap electron_cand(20, 2.47);
    const MuonCandidates muon_cand(20, 2.5);
    const JetSelectionsATLAS_monolep jets_cand;
    AnalysisSelections atlas_selections ({&electron_cand, &muon_cand, &jets_cand});

    /// all the cuts for the analysis
    const LeadingElectronCut leadElectronCut;
    const AddLeptonVetoElectronChannel leptonsVeto;
    const MissingETCut metcuts(65);
    const TransverseMassCut mtcut (130, true);
    const AnalysisCuts monoe_cuts ({&leadElectronCut, &leptonsVeto, &metcuts, &metcuts, &mtcut});

    /// default way to store the information about the event
    EventDataATLAS_monolep event_data;

    /// handles the loop over all the event
    EventLoop event_loop;
    event_loop.setEventData(&event_data);

    /// handles the event analysis
    EventAnalysis atlas_analysis;
    atlas_analysis.setObjectSelection(&atlas_selections);
    atlas_analysis.setCuts(&monoe_cuts);

    /// root to the folder
    TString simulation_folder = "/home/martines/work/MG5_aMC_v3_1_1/PhD/DY/atlas-monoe-13TEV/recast/";
    /// all the masses
    vector<TString> folders = {"300", "600", "1000", "1500", "2000", "3000", "6000"};

    for (auto simulation: folders) { 
        cout << "Running on file " << simulation_folder + simulation << endl;
        /// adds the file for the analysis
        event_loop.addFile(simulation_folder + simulation + "/delphes_default.root");
        /// runs the analysis of the event
        long double efficiency = event_loop.run(&atlas_analysis);
        /// reset the object for the next analysis
        event_loop.reset();
        cout << setprecision(10) << "Efficiency " << efficiency << endl;
    }
    
    return 0;
}