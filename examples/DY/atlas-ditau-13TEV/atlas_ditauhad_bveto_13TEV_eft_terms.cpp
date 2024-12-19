#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <fstream>
#include "TString.h"
#include "DelphesAnalysis/EventLoop.h"
#include "DelphesAnalysis/EventData.h"
#include "DelphesAnalysis/EventAnalysis.h"
#include "DelphesAnalysis/Distribution.h"
#include "DelphesAnalysis/Utilities.h"
#include "EventDataATLAS.h"
#include "SelectionsATLAS-ditau.h"
#include "JSON/json.hpp"

using namespace std;
using json = nlohmann::json;


int main () {
    /// Particle selections for the ATLAS analysis
    const ElectronCandidatesATLAS electron_selection;
    const MuonCandidatesATLAS muon_selection;
    const JetsATLAS jets;
    const HadronicTaus taus_had;
    const AnalysisSelections atlas_ditau_selections ({&electron_selection, &muon_selection, &jets, &taus_had});

    /// All the cuts for the analysis
    const NumberOfLeptons leptons_veto(0);
    const HadronicTausCut hadronic_ditaus_evts;
    const bVeto bveto_evts;
    AnalysisCuts b_veto_2tauhad ({&leptons_veto, &hadronic_ditaus_evts, &bveto_evts});

    /// Default way to store the information about the event
    EventDataATLAS event_data;
    
    /// Handles the loop over all the event
    EventLoop event_loop;
    event_loop.setEventData(&event_data);

    /// Handles the event analysis
    EventAnalysis atlas_analysis;
    atlas_analysis.setObjectSelection(&atlas_ditau_selections);
    atlas_analysis.setCuts(&b_veto_2tauhad);

    /// Set up the distribution
    vector<double> bin_edges = {150., 200., 250., 300., 350., 400., 450., 500., 600., 700., 800., 900., 1000., 1150., 1500000.};
    TransverseMassDiTauHad mt_atlas;
    ObservableDistribution transverse_mass_dist (bin_edges, &mt_atlas);

    /// Folder where the simulations are stored
    const string simulations_folders = "/home/martines/work/MG5_aMC_v3_1_1/PhD/High-PT/atlas-ditau-13TEV/SMEFT-v2-3323/";
    
    /// EFT terms that we simulated 
    vector<string> eft_terms = {
    //    "C1lq-C1lq2D", "C1lq-C2lq2D", "C1lq2D-C1lq2D", "C1lq2D-C2lq2D", "C2lq2D-C2lq2D"
        //   "NP6", "NP8", 
          "NP6-NP6", "NP6-NP8", "NP8-NP8"
    };

    /// Map to stores the results of the analysis
    map<string, vector<double>> analysis_results;
    
    string flavor_indices = "3322";

    /// Mapping among the terms in the EFT expansion and the terms we simulated
    map<string, string> eft_terms_map = {
        {"NP6" + flavor_indices, "NP6"},
        {"NP8" + flavor_indices, "NP6"},
        {"NP6" + flavor_indices + "-NP6" + flavor_indices, "NP6-NP6"},
        {"NP6" + flavor_indices + "-NP8" + flavor_indices, "NP6-NP8"},
        {"NP8" + flavor_indices + "-NP8" + flavor_indices, "NP8-NP8"}
        // dim-8 terms
        // {"C1lq2D3333", "C1lq2D"},
        // {"C3lq2D3333", "C1lq2D"},
        // {"C2lq2D3333", "C2lq2D"},
        // {"C4lq2D3333", "C2lq2D"},
        // dim-6 x dim-8
        // {"C1lq3323-C1lq2D3323", "C1lq-C1lq2D"},
        // {"C1lq3323-C3lq2D3323", "C1lq-C1lq2D"},
        // {"C3lq3323-C1lq2D3323", "C1lq-C1lq2D"},
        // {"C3lq3323-C3lq2D3323", "C1lq-C1lq2D"},
        // {"C1lq3323-C2lq2D3323", "C1lq-C2lq2D"},
        // {"C1lq3323-C4lq2D3323", "C1lq-C2lq2D"},
        // {"C3lq3323-C2lq2D3323", "C1lq-C2lq2D"},
        // {"C3lq3323-C4lq2D3323", "C1lq-C2lq2D"},
        // dim-8 x dim-8
        // {"C1lq2D3323-C1lq2D3323", "C1lq2D-C1lq2D"},
        // {"C1lq2D3323-C2lq2D3323", "C1lq2D-C2lq2D"},
        // {"C1lq2D3323-C3lq2D3323", "C1lq2D-C1lq2D"}, // needs a factor of 2
        // {"C1lq2D3323-C4lq2D3323", "C1lq2D-C2lq2D"},
        // {"C2lq2D3323-C2lq2D3323", "C2lq2D-C2lq2D"},
        // {"C2lq2D3323-C3lq2D3323", "C1lq2D-C2lq2D"},
        // {"C2lq2D3323-C4lq2D3323", "C2lq2D-C2lq2D"}, // needs a factor of 2
        // {"C3lq2D3323-C3lq2D3323", "C1lq2D-C1lq2D"},
        // {"C3lq2D3323-C4lq2D3323", "C1lq2D-C2lq2D"},
        // {"C4lq2D3323-C4lq2D3323", "C2lq2D-C2lq2D"}
    };


    /// Total number of bins for each simulation
    int nbins = 15;

    // json file to store the output of the simulations 
    json results_json;

    /// Loop over all EFT terms
    for (const auto& term: eft_terms) {

        /// Iterates over all the bins
        for (int i = 1; i <= nbins; i++) {

            /// Path to the .root file 
            TString root_file = simulations_folders + "root_files/atlas-ditau-" + term + "-" + to_string(i) + ".root";

            /// path to the banner that stores the cross-section
            string bannerfile = simulations_folders + "lhe_files/atlas-ditau-" + term + "-" + to_string(i) + ".lhe";

            cout << endl << "Analysing file " << root_file << endl;
            
            /// Adds the file to the event loop run
            event_loop.addFile(root_file);

            /// copying the distribution
            shared_ptr<Distribution> current_dist_ptr = transverse_mass_dist.clone();

            /// Launch the analysis - it returns the total number of evts in the file
            int number_of_evts = event_loop.run(&atlas_analysis, current_dist_ptr);

            /// reading the cross-section
            double xsection = read_weight(bannerfile);    

            cout << "sigma (pb): " << xsection << endl;

            /// Weight for the events
            double weight = xsection * 1000 * 139. / number_of_evts;

            /// rescaling the result by the weight
            current_dist_ptr->rescaleDist(weight);

            /// adds the content to the final distribution
            transverse_mass_dist += dynamic_cast<ObservableDistribution&>(*current_dist_ptr);

            /// reset the object for the next analysis
            event_loop.reset();
        }
        cout << "Final (weighted) distribution:" << endl;
        transverse_mass_dist.displayNumberOfEvents();
        /// saving the distribution in the json object
        // results_json[term] = transverse_mass_dist.getBinsContent();
        analysis_results[term] = transverse_mass_dist.getBinsContent();
        /// clear the distribution for the next term
        transverse_mass_dist.clear();
    }

    /// Generates the json file 
    for (const auto& [eft_term, simulation_name]: eft_terms_map) {
        // vector<double> predictions;
        // /// Checks if we need to correct the cross-section by a factor of 2 
        // if (eft_term == "C1lq2D3323-C3lq2D3323" || eft_term == "C2lq2D3323-C4lq2D3323") {
        //     for (auto& entry: analysis_results[simulation_name])
        //         predictions.push_back(2 * entry);
        // }
        // else 
        //     predictions = analysis_results[simulation_name];
        
        results_json[eft_term] = analysis_results[simulation_name];
    }


    /// Saves .json file
    // save json file
    ofstream file("atlas-ditau-bveto-13TEV-v2-3323.json");
    if (file.is_open()) {
        file << results_json.dump(4); // Pretty print with 4 spaces indentation
        file.close();
        cout << "JSON file created successfully!" << std::endl;
    }

    return 0;
}