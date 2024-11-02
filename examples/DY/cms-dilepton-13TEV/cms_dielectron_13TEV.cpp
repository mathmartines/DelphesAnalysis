#include <iostream>
#include <vector>
#include "TString.h"
#include "DelphesAnalysis/EventLoop.h"
#include "DelphesAnalysis/EventData.h"
#include "DelphesAnalysis/EventAnalysis.h"
#include "EventDataCMS_dilepton.h"
#include "SelectionsCMS_dilepton.h"


using namespace std;

int main() {
    /// particle selections for the ATLAS analysis
    const ElectronCandidates electron_selection;

    /// all the cuts for the analysis
    const NumberOfElectrons dielectron_events(2);

    /// default way to store the information about the event
    EventDataCMS_dielectron event_data;

    /// handles the loop over all the event
    EventLoop event_loop;
    event_loop.setEventData(&event_data);

    /// handles the event analysis
    EventAnalysis cms_analysis;
    cms_analysis.setObjectSelection(&electron_selection);
    cms_analysis.setCuts(&dielectron_events);

    /// root to the folder
    TString simulation_folder = "/home/martines/work/MG5_aMC_v3_1_1/PhD/DY/cms-dielectron-13TEV/recast/gl-gl-gq-gq_";
    /// all the masses
    vector<TString> folders = {
        "300_GeV/Events/run_02",
        "400_GeV/Events/run_02", 
        "600_GeV/Events/run_02", 
        "1000_GeV/Events/run_02", 
        "1500_GeV/Events/run_02", 
        "2000_GeV/Events/run_02", 
        "2500_GeV/Events/run_02",
        "3000_GeV/Events/run_02"
    };

    for (auto simulation: folders) { 
        cout << "Running on file " << simulation_folder + simulation << endl;
        /// adds the file for the analysis
        event_loop.addFile(simulation_folder + simulation + "/delphes_10.root");
        /// runs the analysis of the event
        long double efficiency = event_loop.run(&cms_analysis);
        /// reset the object for the next analysis
        event_loop.reset();
        cout << setprecision(10) << "Efficiency " << efficiency << endl;
    }
    
    return 0;
}