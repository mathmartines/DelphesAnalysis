#include <iostream>
#include <vector>
#include "TString.h"
#include "DelphesAnalysis/ObjectSelection.h"
#include "DelphesAnalysis/EventLoop.h"
#include "DelphesAnalysis/EventData.h"
#include "DelphesAnalysis/Analysis.h"
#include "EventDataCMS_monolep.h"
#include "SelectionsCMS_monolep.h"


using namespace std;

int main() {
    /// particle selections for the ATLAS analysis
    const ElectronCandidates electron_cand;
    const MuonCandidates muon_cand;
    const Jets jets_cand;
    AnalysisSelections cms_monolep_selections ({&electron_cand, &muon_cand, &jets_cand});

    /// all the cuts for the analysis
    const NumberOfLeptons nleptons;
    const MuonEvent muons_evts;
    const AdditionalLeptonsVeto leptons_veto (false);
    const VetoOnJets jetsveto;
    const bJetVeto bjetveto;
    const MissingETCuts metcuts;
    const AnalysisCuts monomu_cuts ({&nleptons, &muons_evts, &leptons_veto, &jetsveto, &bjetveto, &metcuts});

    /// default way to store the information about the event
    EventDataCMS_monolepton event_data;

    /// handles the loop over all the event
    EventLoop event_loop;
    event_loop.setEventData(&event_data);

    /// handles the event analysis
    Analysis cms_analysis;
    cms_analysis.setObjectSelection(&cms_monolep_selections);
    cms_analysis.setCuts(&monomu_cuts);

    /// root to the folder
    TString simulation_folder = "/home/martines/work/MG5_aMC_v3_1_1/PhD/DY/cms-monomu-13TEV/recast/";
    /// all the masses
    vector<TString> folders = {
        "400", 
        "600", 
        "1000", 
        "1600", 
        "2000", 
        "3000",
        "4000"
    };

    for (auto simulation: folders) { 
        cout << "Running on file " << simulation_folder + simulation << endl;
        /// adds the file for the analysis
        event_loop.addFile(simulation_folder + simulation + "/delphes4.root");
        /// runs the analysis of the event
        long double efficiency = event_loop.run(&cms_analysis);
        /// reset the object for the next analysis
        event_loop.reset();
        cout << setprecision(10) << "Efficiency " << efficiency << endl;
    }
    
    return 0;
}