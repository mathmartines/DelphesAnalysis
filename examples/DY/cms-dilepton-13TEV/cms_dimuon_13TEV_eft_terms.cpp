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
    /// particle selections for the ATLAS analysis
    const MuonCandidatesCMS muon_selections;

    /// all the cuts for the analysis
    const NumberOfMuons dimuon_events(2);
    const InvariantMassCut invariant_mass_muons;
    const OppositeChargeCut opposite_charge;
    const AngularDistanceCut angle_cut;
    const AnalysisCuts cms_dimuon_cuts ({&dimuon_events, &opposite_charge, &angle_cut});

    /// default way to store the information about the event
    EventDataCMS_dimuon event_data;

    /// defining the observable for the distribution
    vector<double> bin_edges = {
        120.0, 266.2, 288.3, 312.3, 338.2, 366.2, 396.6, 429.5, 465.1, 503.7, 545.5, 
        590.7, 639.8, 692.8, 750.3, 812.5, 879.9, 952.9, 1032.0, 1117.6, 
        1210.3, 1310.7, 1419.4, 1537.2, 1664.7, 1802.8, 1952.4, 2114.3, 2289.7, 
        2479.7, 2685.4, 2908.1, 3149.4, 3410.7, 3693.6, 4000.0, 4500.0, 5200.0,
        6000.0, 7000.0
    };

    DielectronInvariantMass invariant_mass;
    ObservableDistribution invm_dist (bin_edges, &invariant_mass);

    /// handles the loop over all the event 
    EventLoop event_loop;
    event_loop.setEventData(&event_data);

    /// handles the event analysis
    EventAnalysis cms_analysis;
    cms_analysis.setObjectSelection(&muon_selections);
    cms_analysis.setCuts(&cms_dimuon_cuts);

    /// root to the folder
    string simulation_folder = "/home/martines/work/MG5_aMC_v3_1_1/PhD/DY/cms-dielectron-13TEV/UniversalSMEFT_d8/";
    /// all the masses
    vector<string> eft_terms = {
        "SM", 
        // tilda variables
        "cphi1T", "D4FT", "cBWT", 
        "cphi1T-cphi1T", "cphi1T-D4FT", "cphi1T-cBWT", "D4FT-D4FT", "D4FT-cBWT", "cBWT-cBWT",
        // d6 coefficients
        "c2JB", "c2JW", "c2JB-c2JW", "c2JW-c2JW",
        // d6 x renorm
        "c2JB-c2JWrenorm", "c2JB-cBW", "c2JB-cphi1", "c2JW-c2JWrenorm", "c2JW-cBW", "c2JW-cphi1",
        // d8 terms
        "c1psi2H2D3", "c2psi2H2D3", "c5psi4H2", "c4psi4H2", "c7psi4H2", "c2psi4D2", "c3psi4D2"
    };

    // list of d8 coefficients
    vector<string> d8coefs = {"c1psi2H2D3", "c2psi2H2D3", "c5psi4H2", "c4psi4H2", "c7psi4H2", "c2psi4D2", "c3psi4D2"};

    /// number of bins in the folder 
    int nbins = 29;

    // json file to store the output of the simulations
    json results_json;

    /// runs the analysis in all the simulations
    for (auto eft_term: eft_terms) {

        for (int bin_index = 1; bin_index <= nbins; bin_index++) {
            /// path to the root file
            TString rootfile = simulation_folder + eft_term + "/bin_" + to_string(bin_index) + "/Events/run_01/delphes_events_final_muons.root";
            /// path to the banner that stores the cross-section
            string bannerfile = simulation_folder + eft_term + "/bin_" + to_string(bin_index) + "/Events/run_01/run_01_tag_1_banner.txt";

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
            double weight = xsection * 1000. * 140. / number_evts;
            /// if it's a dim-8 coeff I need to add another 1/TeV^2 factor
            if (find(d8coefs.begin(), d8coefs.end(), eft_term) != d8coefs.end())
                weight *= 1.0E-06;
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
    ofstream file("cms-dimuon-13TEV.json");
    if (file.is_open()) {
        file << results_json.dump(4); // Pretty print with 4 spaces indentation
        file.close();
        cout << "JSON file created successfully!" << std::endl;
    }

    return 0;
}   