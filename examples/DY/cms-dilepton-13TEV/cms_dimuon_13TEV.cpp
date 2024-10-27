#include <iostream>
#include <vector>
#include "TString.h"
#include "DelphesAnalysis/EventLoop.h"
#include "DelphesAnalysis/EventData.h"
#include "DelphesAnalysis/Analysis.h"
#include "EventDataCMS_dilepton.h"
#include "SelectionsCMS_dilepton.h"


using namespace std;

int main() {
    /// particle selections for the ATLAS analysis
    const MuonCandidates muon_selection;

    /// all the cuts for the analysis
    const NumberOfMuons dimuon_events(2);
    const InvariantMassCut invariant_mass_muons;
    const OppositeChargeCut opposite_charge;
    const AngularDistanceCut angle_cut;

    const AnalysisCuts cms_dimuon_cuts ({&dimuon_events, &invariant_mass_muons, &opposite_charge, &angle_cut});

    /// default way to store the information about the event
    EventDataCMS_dimuon event_data;

    /// handles the loop over all the event
    EventLoop event_loop;
    event_loop.setEventData(&event_data);

    /// handles the event analysis
    Analysis cms_analysis;
    cms_analysis.setObjectSelection(&muon_selection);
    cms_analysis.setCuts(&cms_dimuon_cuts);

    /// root to the folder
    TString simulation_folder = "/home/martines/work/MG5_aMC_v3_1_1/PhD/DY/cms-dimuon-13TEV/recast/gl-gl-gq-gq_";
    /// all the masses
    vector<TString> folders = {
        "300_GeV/Events/run_03",
        "400_GeV/Events/run_03", 
        "600_GeV/Events/run_03", 
        "1000_GeV/Events/run_03", 
        "1500_GeV/Events/run_03", 
        "2000_GeV/Events/run_03", 
        "2500_GeV/Events/run_03",
        "3000_GeV/Events/run_03"
    };

    for (auto simulation: folders) { 
        cout << "Running on file " << simulation_folder + simulation << endl;
        /// adds the file for the analysis
        event_loop.addFile(simulation_folder + simulation + "/delphes_7.root");
        /// runs the analysis of the event
        long double efficiency = event_loop.run(&cms_analysis);
        /// reset the object for the next analysis
        event_loop.reset();
        cout << setprecision(10) << "Efficiency " << efficiency << endl;
    }
    
    return 0;
}