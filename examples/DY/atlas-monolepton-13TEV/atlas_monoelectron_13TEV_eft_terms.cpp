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
    const LeadingElectronCut leadElectronCut;
    const AddLeptonVetoElectronChannel leptonsVeto;
    const MissingETCut metcuts(65);
    const TransverseMassCut mtcut (130, true);
    const AnalysisCuts monoe_cuts ({&leadElectronCut, &leptonsVeto, &metcuts, &mtcut});

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
        130.0, 138.982, 148.585, 158.852, 169.828, 181.562, 194.107, 207.519, 221.857, 
        237.186, 253.575, 271.095, 289.827, 309.852, 331.261, 354.15, 378.62, 404.781, 
        432.749, 462.65, 494.616, 528.792, 565.329, 604.39, 646.15, 690.796, 738.526, 
        789.555, 844.109, 902.433, 964.786, 1031.45, 1102.72, 1178.91, 1260.36, 1347.45, 
        1440.55, 1540.09, 1646.5, 1760.26, 1881.89, 2011.92, 2150.93, 2299.55, 2458.43, 
        2628.3, 2809.9, 3004.05, 3211.62, 3433.52, 3670.76, 3924.39, 4195.55, 4485.44, 
        4795.36, 5126.69, 5480.92, 5859.63, 6264.5, 6697.34, 7160.09, 7654.82, 8183.73, 
        8749.18, 9353.71, 10000.0
    };
    TransverseMassEventObs<Electron> mt_calc ("Electron");
    ObservableDistribution mt_dist(bin_edges, &mt_calc);

    /// root to the folder
    string simulation_folder = "/home/martines/work/MG5_aMC_v3_1_1/PhD/DY/atlas-monoelectron-13TEV/";
   
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
            TString rootfile = simulation_folder + "root_files/atlas-monoelectron-" + eft_term + "-" + to_string(bin_index) + ".root";
            /// path to the banner that stores the cross-section
            string bannerfile = simulation_folder + "lhe_files/atlas-monoelectron-" + eft_term + "-" + to_string(bin_index) + ".lhe";

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
    ofstream file("atlas-monoelectron-13TEV.json");
    if (file.is_open()) {
        file << results_json.dump(4); // Pretty print with 4 spaces indentation
        file.close();
        cout << "JSON file created successfully!" << std::endl;
    }
    
    return 0;
}   