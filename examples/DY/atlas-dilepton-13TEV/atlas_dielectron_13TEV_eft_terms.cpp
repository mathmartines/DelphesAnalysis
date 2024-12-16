#include <iostream>
#include <vector>
#include "TString.h"
#include "JSON/json.hpp"
#include "DelphesAnalysis/ObjectSelection.h"
#include "DelphesAnalysis/EventLoop.h"
#include "DelphesAnalysis/EventData.h"
#include "DelphesAnalysis/EventAnalysis.h"
#include "DelphesAnalysis/Observable.h"
#include "DelphesAnalysis/Distribution.h"
#include "EventDataATLAS_dilepton.h"
#include "SelectionsATLAS_dilepton.h"
#include "DelphesAnalysis/Utilities.h"

using json = nlohmann::json;
using namespace std;

int main() {
    /// particle selections for the ATLAS analysis
    const ElectronCandidatesATLAS electron_candidates;
    const MuonCandidatesATLAS muon_candidates;
    AnalysisSelections atlas_selections ({&electron_candidates, &muon_candidates});

    /// all the cuts for the analysis
    const NumberOfElectrons nelectrons_cut;
    const AnalysisCuts dielectron_cuts ({&nelectrons_cut});

    /// default way to store the information about the event
    EventDataATLAS_dileptons event_data;

    /// handles the loop over all the event
    EventLoop event_loop;
    event_loop.setEventData(&event_data);

    /// handles the event analysis
    EventAnalysis atlas_analysis;
    atlas_analysis.setObjectSelection(&atlas_selections);
    atlas_analysis.setCuts(&dielectron_cuts);

    /// bins of the distribution
    vector<double> bin_edges = {
        130.00, 135.08, 140.35, 145.84, 151.53, 157.45, 163.60, 
        170.00, 176.64, 183.54, 190.71, 198.16, 205.90, 213.94, 
        222.30, 230.98, 240.00, 249.38, 259.12, 269.24, 279.76, 
        290.69, 302.04, 313.84, 326.10, 338.84, 352.08, 365.83, 
        380.12, 394.97, 410.40, 426.43, 443.09, 460.40, 478.38, 
        497.07, 516.49, 536.66, 557.63, 579.41, 602.04, 625.56, 
        650.00, 675.39, 701.77, 729.18, 757.67, 787.27, 818.02, 
        849.97, 883.18, 917.68, 953.52, 990.77, 1029.50, 1069.70, 
        1111.50, 1154.90, 1200.00, 1246.90, 1295.60, 1346.20, 
        1398.80, 1453.40, 1510.20, 1569.20, 1630.50, 1694.20, 
        1760.40, 1829.10, 1900.60, 1974.80, 2052.00, 2132.10, 
        2215.40, 2302.00, 2391.90, 2485.30, 2582.40, 2683.30, 
        2788.10, 2897.00, 3010.20, 3127.80, 3250.00, 3376.90, 
        3508.80, 3645.90, 3788.30, 3936.30, 4090.10, 4249.80, 
        4415.90, 4588.40, 4767.60, 4953.80, 5147.30, 5348.40, 
        5557.30, 5774.40, 6000.00
    };
    DielectronInvariantMass ee_invmass;
    ObservableDistribution invm_dist (bin_edges, &ee_invmass);

    /// root to the folder
    string simulation_folder = "/home/martines/work/MG5_aMC_v3_1_1/PhD/DY/atlas-dielectron-13TEV/";
    /// all the masses
    vector<string> eft_terms = {
        "SM"
    };

    // json file to store the output of the simulations
    json results_json;

     /// runs the analysis in all the simulations
    for (auto eft_term: eft_terms) {

        for (int bin_index = 1; bin_index < 10; bin_index++) {
            /// path to the root file
            TString rootfile = simulation_folder + "root_files/atlas-dielectron-" + eft_term + "-" + to_string(bin_index) + ".root";
            /// path to the banner that stores the cross-section
            string bannerfile = simulation_folder + "lhe_files/atlas-dielectron-" + eft_term + "-" + to_string(bin_index) + ".lhe";

            cout << "Analysing file " << rootfile << endl;

            /// adding the file to the event loop run
            event_loop.addFile(rootfile);

            /// copying the distribution
            shared_ptr<Distribution> current_dist_ptr = invm_dist.clone();

            /// run the event analysis
            int number_evts = event_loop.run(&atlas_analysis, current_dist_ptr);
           
            /// reading the cross-section
            double xsection = read_weight(bannerfile);       
            cout << "Cross-section: " << xsection << endl;
           
            /// calculating the weight
            double weight = xsection * 1000. * 139. / number_evts;

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
    ofstream file("atlas-dielectron-13TEV.json");
    if (file.is_open()) {
        file << results_json.dump(4); // Pretty print with 4 spaces indentation
        file.close();
        cout << "JSON file created successfully!" << std::endl;
    }
    
    return 0;
}