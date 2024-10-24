#include <iostream>
#include <vector>
#include "TString.h"
#include "DelphesAnalysis/EventLoop.h"
#include "DelphesAnalysis/EventData.h"
#include "DelphesAnalysis/Cut.h"
#include "DelphesAnalysis/Analysis.h"
#include "SelectionsATLAS-ditau.h"
#include "AnalysisATLAS-ditau.h"

using namespace std;

int main() {
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

    /// analyses
    ATLAS_ditau_had_bveto ditau_had_bveto;
    ATLAS_ditau_had_btag ditau_had_btag;
    ATLAS_taulep_tauhad_bveto taulep_tauhad_bveto;
    ATLAS_taulep_tauhad_btag taulep_tauhad_btag;
    vector<Analysis*> all_analysis = {
        // &ditau_had_bveto,
        // &ditau_had_btag,
        &taulep_tauhad_bveto, 
        &taulep_tauhad_btag
    };

    /// launch the analysis
    for (auto analysis: all_analysis) {
        for (auto simulation: folders) {
            cout << "Launching analysis " << analysis->getAnalysisName() << " in file for mass " << simulation << "..." << endl;
                analysis->runAnalysis(simulation_folder + simulation + "/delphes.root"); 
        }
    }
    
    return 0;
}