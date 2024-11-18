#include <iostream>
#include <vector>
#include "JSON/json.hpp"
#include "TString.h"
#include "DelphesAnalysis/ObjectSelection.h"
#include "DelphesAnalysis/EventLoop.h"
#include "DelphesAnalysis/EventData.h"
#include "DelphesAnalysis/EventAnalysis.h"
#include "DelphesAnalysis/Observable.h"
#include "DelphesAnalysis/Distribution.h"
#include "EventDataCMS_monolep.h"
#include "SelectionsCMS_monolep.h"
#include "DelphesAnalysis/Utilities.h"

using namespace std;
using json = nlohmann::json;

int main() {
    /// particle selections for the ATLAS analysis
    // const ElectronCandidatesCMS electron_cand;
    const MuonCandidatesCMS muon_cand;
    const JetsCMS jets_cand;
    AnalysisSelections cms_monolep_selections ({&muon_cand, &jets_cand});

    /// all the cuts for the analysis
    const NumberOfLeptons nleptons;
    const MuonEvent muons_evts;
    const AdditionalLeptonsVeto leptons_veto (true);
    const VetoOnJets jetsveto;
    const bJetVeto bjetveto;
    const MissingETCuts metcuts;
    const AnalysisCuts monoe_cuts ({
        &nleptons,
        &muons_evts, 
        &leptons_veto, 
        &jetsveto, 
        &bjetveto,
        &metcuts
    });

    /// defining the analysis
    EventAnalysis cms_analysis;
    cms_analysis.setObjectSelection(&cms_monolep_selections);
    cms_analysis.setCuts(&monoe_cuts);

    /// default way to store the information about the event
    EventDataCMS_monolepton event_data;

    /// bins of the distribution
    vector<double> bin_edges;
    for (int i = 0; i < 71; i++) bin_edges.push_back(120. + i * 98.);
    TransverseMassEventObs mt_calc;
    ObservableDistribution mt_dist(bin_edges, &mt_calc);

    /// root to the folder
    string simulation_folder = "/home/martines/work/MG5_aMC_v3_1_1/PhD/DY/cms-monoe-13TEV/UniversalSMEFT_d8/";
    /// all the masses
    vector<string> eft_terms = {"SM"};

    /// handles the loop over all the event
    EventLoop event_loop;
    event_loop.setEventData(&event_data);

    // json file to store the output of the simulations
    json results_json;

     /// runs the analysis in all the simulations
    for (auto eft_term: eft_terms) {

        for (int bin_index = 1; bin_index <= 7; bin_index++) {
            /// path to the root file
            TString rootfile = simulation_folder + eft_term + "/bin_" + to_string(bin_index) + "/Events/run_01/delphes_events_muons.root";
            /// path to the banner that stores the cross-section
            string bannerfile = simulation_folder + eft_term + "/bin_" + to_string(bin_index) + "/Events/run_01/run_01_tag_1_banner.txt";

            cout << "Analysing file " << rootfile << endl;

            /// adding the file to the event loop run
            event_loop.addFile(rootfile);

            /// copying the distribution
            shared_ptr<Distribution> current_dist_ptr = mt_dist.clone();

            /// run the event analysis
            int number_evts = event_loop.run(&cms_analysis, current_dist_ptr);
            /// reading the cross-section
            double xsection = read_weight(bannerfile);       
            cout << "Cross-section: " << xsection << endl;
            /// calculating the weight
            double weight = xsection * 1000. * 138. / number_evts;
        
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
        std::vector<double> dist = mt_dist.getBinsContent();
        for (int i = 0; i < dist.size(); i++) {
            cout << bin_edges[i] << "-" << bin_edges[i + 1] << ": " << dist[i] << endl; 
        }

        results_json[eft_term] = mt_dist.getBinsContent();
        /// clear the distribution for the next term
        mt_dist.clear();
    }
    
     // save json file
    ofstream file("cms-monomu-13TEV.json");
    if (file.is_open()) {
        file << results_json.dump(4); // Pretty print with 4 spaces indentation
        file.close();
        cout << "JSON file created successfully!" << std::endl;
    }

    return 0;
}