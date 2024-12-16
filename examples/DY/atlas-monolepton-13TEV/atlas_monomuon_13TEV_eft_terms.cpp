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
#include "EventDataATLAS_monolep.h"
#include "SelectionsATLAS_monolep.h"
#include "DelphesAnalysis/Utilities.h"

using json = nlohmann::json;
using namespace std;

int main() {
    /// particle selections for the ATLAS analysis
    const ElectronCandidatesATLAS electron_cand;
    const MuonCandidatesATLAS muon_cand;
    const JetCandidatesATLAS jets_cand;
    AnalysisSelections atlas_selections ({&electron_cand, &muon_cand, &jets_cand});

    /// all the cuts for the analysis
    const LeadingMuonCut leadMuonCut;
    const AddLeptonVetoMuonChannel leptonsVeto;
    const MissingETCut metcuts(55);
    const TransverseMassCut mtcut (110, false);
    const AnalysisCuts monoe_cuts ({
        &leadMuonCut, 
        &leptonsVeto, 
        &metcuts, 
        &metcuts, 
        &mtcut
    });

    /// default way to store the information about the event
    EventDataATLAS_monolep event_data;

    /// handles the loop over all the event
    EventLoop event_loop;
    event_loop.setEventData(&event_data);

    /// handles the event analysis
    EventAnalysis atlas_analysis;
    atlas_analysis.setObjectSelection(&atlas_selections);
    atlas_analysis.setCuts(&monoe_cuts);

    /// bins of the distribution
    vector<double> bin_edges = {
        110.0, 119.581, 129.997, 141.32, 153.63, 167.011, 181.558, 197.372, 
        214.564, 233.253, 253.57, 275.657, 299.667, 325.769, 354.144, 384.991, 
        418.524, 454.979, 494.609, 537.69, 584.525, 635.438, 690.786, 750.956, 
        816.366, 887.473, 964.775, 1048.81, 1140.16, 1239.47, 1347.44, 1464.8,
        1592.39, 1731.09, 1881.87, 2045.79, 2223.98, 2417.69, 2628.28, 2857.21, 
        3106.08, 3376.63, 3670.74, 3990.47, 4338.05, 4715.91, 5126.68, 5573.22, 
        6058.67, 6586.39, 7160.08, 7783.74, 8461.73, 9198.76, 10000.0
    };
    TransverseMassEventObs<Muon> mt_calc ("Muon");
    ObservableDistribution mt_dist(bin_edges, &mt_calc);

    /// root to the folder
    string simulation_folder = "/home/martines/work/MG5_aMC_v3_1_1/PhD/DY/";
    /// all the masses
    vector<string> eft_terms = {
        "SM", 
        "cphi1T", "cphi1T-cphi1T", "cphi1T-D4FT", "cphi1T-cBWT", "D4FT-D4FT", "D4FT-cBWT", "cBWT-cBWT",
        "c2JW", "c2JW-c2JW", "c2JW-cphi1", "c3psi4D2", "c2psi2H2D3"
    };

    // json file to store the output of the simulations
    json results_json;

     /// runs the analysis in all the simulations
    for (auto eft_term: eft_terms) {

        for (int bin_index = 1; bin_index <= 10; bin_index++) {
            /// path to the root file
            TString rootfile = simulation_folder + "atlas-monomuon-13TEV/root_files/atlas-monomuon-" + eft_term + "-" + to_string(bin_index) + ".root";
            /// path to the banner that stores the cross-section
            string bannerfile = simulation_folder + "atlas-monomuon-13TEV/lhe_files/atlas-monomuon-" + eft_term + "-" + to_string(bin_index) + ".lhe";

            cout << "Analysing file " << rootfile << endl;

            /// adding the file to the event loop run
            event_loop.addFile(rootfile);

            /// copying the distribution
            shared_ptr<Distribution> current_dist_ptr = mt_dist.clone();

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
            mt_dist += dynamic_cast<ObservableDistribution&>(*current_dist_ptr);

            /// reset the object for the next analysis
            event_loop.reset();
        }
        cout << "Final (weighted) distribution:" << endl;
        mt_dist.displayNumberOfEvents();
        /// saving the distribution in the json object
        results_json[eft_term] = mt_dist.getBinsContent();
        /// clear the distribution for the next term
        mt_dist.clear();
    }
    
    // save json file
    ofstream file("atlas-monomu-13TEV.json");
    if (file.is_open()) {
        file << results_json.dump(4); // Pretty print with 4 spaces indentation
        file.close();
        cout << "JSON file created successfully!" << std::endl;
    }
    
    return 0;
}