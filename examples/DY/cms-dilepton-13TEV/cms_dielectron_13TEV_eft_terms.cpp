#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include "JSON/json.hpp"
#include "TString.h"
#include "DelphesAnalysis/EventLoop.h"
#include "DelphesAnalysis/EventData.h"
#include "DelphesAnalysis/EventAnalysis.h"
#include "DelphesAnalysis/Distribution.h"
#include "DelphesAnalysis/Observable.h"
#include "EventDataCMS_dilepton.h"
#include "SelectionsCMS_dilepton.h"
#include "DelphesAnalysis/Utilities.h"

using namespace std;
using json = nlohmann::json;


int main() {
    /// Particle candidates for the analysis
    const ElectronCandidatesCMS electron_selection;
    const AnalysisSelections cms_dielectron_selections ({&electron_selection});

    /// all the cuts for the analysis
    const NumberOfElectrons dielectron_events(2);
    const ElectronInBarrel barrel_region_electron;
    const AnalysisCuts dielectron_cuts ({&dielectron_events, &barrel_region_electron});

    /// default way to store the information about the event
    EventDataCMS event_data;

    /// defining the observable for the distribution
    vector<double> bin_edges = {
        60.0 , 95.0, 240.0, 260.0, 280.0, 300.0, 320.0, 340.0, 360.0, 380.0, 
        400.0, 420.0, 440.0, 460.0, 480.0, 500.0, 520.0, 540.0, 
        560.0, 580.0, 600.0, 630.0, 660.0, 690.0, 720.0, 750.0, 
        780.0, 810.0, 840.0, 870.0, 900.0, 950.0, 1000.0, 1050.0, 
        1100.0, 1150.0, 1200.0, 1250.0, 1310.0, 1370.0, 1430.0, 
        1490.0, 1550.0, 1610.0, 1680.0, 1750.0, 1820.0, 1890.0, 
        1970.0, 2050.0, 2130.0, 2210.0, 2290.0, 2370.0, 2450.0, 
        2530.0, 2610.0, 2690.0, 2770.0, 2850.0, 2930.0, 3010.0, 
        3090.0, 3170.0, 3250.0, 3330.0, 3410.0, 3490.0, 3570.0, 
        3650.0, 3730.0, 3810.0, 3890.0, 3970.0, 4070.0, 4170.0, 
        4270.0, 4370.0, 4470.0, 4570.0, 4670.0, 4770.0, 4870.0, 
        4970.0, 5070.0, 5170.0, 5270.0, 5370.0, 5470.0, 5570.0, 
        5670.0, 5770.0, 5870.0, 5970.0, 6070.0
    };
    DielectronInvariantMass ee_invmass;
    ObservableDistribution invm_dist (bin_edges, &ee_invmass);

    /// handles the loop over all the event
    EventLoop event_loop;
    event_loop.setEventData(&event_data);

    /// handles the event analysis
    EventAnalysis cms_analysis;
    cms_analysis.setObjectSelection(&cms_dielectron_selections);
    cms_analysis.setCuts(&dielectron_cuts);

    /// root to the folder
    string simulation_folder = "/home/martines/work/MG5_aMC_v3_1_1/PhD/DY/cms-dielectron-13TEV/";
    /// all the masses
    vector<string> eft_terms = {
        "SM", "cphi1T", "cBWT", "cphi1T-cphi1T", "cphi1T-D4FT", "cphi1T-cBWT", "D4FT-D4FT", "D4FT-cBWT", "cBWT-cBWT",
        "c2JB", "c2JW", "c2JB-c2JB", "c2JB-c2JW", "c2JW-c2JW", "c2JB-cphi1", "c2JB-cBW", "c2JW-cphi1", "c2JW-cBW",
        "c7psi4H2", "c2psi4D2", "c3psi4D2", "c1psi2H2D3", "c2psi2H2D3"
    };

    // json file to store the output of the simulations
    json results_json;

    /// runs the analysis in all the simulations
    for (auto eft_term: eft_terms) {

        for (int bin_index = 1; bin_index <= 9; bin_index++) {
            /// path to the root file
            TString rootfile = simulation_folder + "root_files/cms-dielectron-" + eft_term + "-" + to_string(bin_index) + ".root";
            /// path to the banner that stores the cross-section
            string bannerfile = simulation_folder + "lhe_files/cms-dielectron-" + eft_term + "-" + to_string(bin_index) + ".lhe";

            cout << "Analysing file " << rootfile << endl;

            /// adding the file to the event loop run
            event_loop.addFile(rootfile);

            /// copying the distribution
            shared_ptr<Distribution> current_dist_ptr = invm_dist.clone();

            /// run the event analysis
            int number_evts = event_loop.run(&cms_analysis, current_dist_ptr);
           
            /// reading the cross-section
            double xsection = read_weight(bannerfile);       
            cout << "Cross-section: " << xsection << endl;
           
            /// calculating the weight
            double weight = xsection * 1000. * 137. / number_evts;
           
            /// rescaling the result by the weight
            /// number of events is not the same for all samples
            current_dist_ptr->rescaleDist(weight);

            /// adds the content to the final distribution
            invm_dist += dynamic_cast<ObservableDistribution&>(*current_dist_ptr);

            /// reset the object for the next analysis
            event_loop.reset();
        }
        cout << "Final (weighted) distribution:" << endl;
        invm_dist.displayNumberOfEvents();
        /// saving the distribution in the json object
        results_json[eft_term] = invm_dist.getBinsContent();
        /// clear the distribution for the next term
        invm_dist.clear();
    }

    // save json file
    ofstream file("cms-dielectron-13TEV.json");
    if (file.is_open()) {
        file << results_json.dump(4); // Pretty print with 4 spaces indentation
        file.close();
        cout << "JSON file created successfully!" << std::endl;
    }

    return 0;
}   