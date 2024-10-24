#include <iostream>
#include <vector>
#include "TString.h"
#include "DelphesAnalysis/EventLoop.h"
#include "DelphesAnalysis/EventData.h"
#include "DelphesAnalysis/Analysis.h"
#include "SelectionsATLAS-ditau.h"

using namespace std;

int main() {
    /// particle selections for the ATLAS analysis
    const ElectronCandidates electron_selection;
    const MuonCandidates muon_selection;
    const Jets jets;
    const HadronicTaus taus_had;
    const AnalysisSelections atlas_ditau_selections ({&electron_selection, &muon_selection, &jets, &taus_had});

    /// all the cuts for the analysis
    const LeptonsVeto leptons_veto;
    const HadronicTausCut hadronic_ditaus_evts;
    const bVeto bveto_evts;
    const bTag btag_events;
    const SingleLeptonCut single_lepton_cut;
    const LeptonPtCut lepton_pt_cut;
    const NumberOfHadronicTaus number_taush;
    const TauLeptonEventsCuts tau_lep_evts_cuts;

    // defining the analysis to be made
    AnalysisCuts b_veto_2tauhad ({&leptons_veto, &hadronic_ditaus_evts, &bveto_evts});
    AnalysisCuts b_tag_2tauhad ({&leptons_veto, &hadronic_ditaus_evts, &btag_events});
    AnalysisCuts b_veto_taulep_tauhad ({&single_lepton_cut, &lepton_pt_cut, &number_taush, &tau_lep_evts_cuts, &bveto_evts});
    AnalysisCuts b_tag_taulep_tauhad ({&single_lepton_cut, &lepton_pt_cut, &number_taush, &tau_lep_evts_cuts, &btag_events});

    /// default way to store the information about the event
    EventData event_data;

    /// handles the loop over all the event
    EventLoop atlas_evt_loop;
    atlas_evt_loop.setEventData(&event_data);

    /// handles the event analysis
    Analysis atlas_ditau_analysis;
    atlas_ditau_analysis.setObjectSelection(&atlas_ditau_selections);

    /// root to the folder
    TString simulation_folder = "/home/martines/work/MG5_aMC_v3_1_1/PhD/DY/ditau-ATLAS13TEV/recast/cHgg-cHgg_";
    /// all the masses
    vector<TString> folders = {
        "200GeV/Events/run_02", 
        "300GeV/Events/run_02",
        "400GeV/Events/run_02", 
        "600GeV/Events/run_02", 
        "1000GeV/Events/run_01", 
        "1500GeV/Events/run_01", 
        "2000GeV/Events/run_01", 
        "2500GeV/Events/run_01"
    };

    for (auto simulation: folders) { 
        cout << "Running on file " << simulation_folder + simulation << endl;
        /// adds the file for the analysis
        atlas_evt_loop.addFile(simulation_folder + simulation + "/delphes.root");
        /// defines the cuts
        atlas_ditau_analysis.setCuts(&b_tag_taulep_tauhad);
        /// runs the analysis of the event
        long double efficiency = atlas_evt_loop.run(&atlas_ditau_analysis);
        /// reset the object for the next analysis
        atlas_evt_loop.reset();
        cout << setprecision(10) << "Efficiency " << efficiency << endl;
    }
    
    return 0;
}