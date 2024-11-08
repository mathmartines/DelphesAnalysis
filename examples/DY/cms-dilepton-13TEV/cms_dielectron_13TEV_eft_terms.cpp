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
    const ElectronCandidatesCMS electron_selection;

    /// all the cuts for the analysis
    const NumberOfElectrons dielectron_events(2);

    /// default way to store the information about the event
    EventDataCMS_dielectron event_data;

    /// defining the observable for the distribution
    vector<double> bin_edges = {60, 120, 400, 600, 900, 1300, 1800, 13000};
    DielectronInvariantMass ee_invmass;
    ObservableDistribution invm_dist (bin_edges, &ee_invmass);

    /// handles the loop over all the event
    EventLoop event_loop;
    event_loop.setEventData(&event_data);

    /// handles the event analysis
    EventAnalysis cms_analysis;
    cms_analysis.setObjectSelection(&electron_selection);
    cms_analysis.setCuts(&dielectron_events);

    /// root to the folder
    string simulation_folder = "/home/martines/work/MG5_aMC_v3_1_1/PhD/DY/cms-dielectron-13TEV/UniversalSMEFT_d8/";
    /// all the masses
    vector<string> eft_terms = {
        "SM", 
        "cphi1T", "D4FT", "cBWT", 
        "cphi1T-cphi1T", "cphi1T-D4FT", "cphi1T-cBWT", "D4FT-D4FT", "D4FT-cBWT", "cBWT-cBWT"
    };

    /// number of bins in the folder 
    int nbins = 29;

    // json file to store the output of the simulations
    json results_json;

    /// runs the analysis in all the simulations
    for (auto eft_term: eft_terms) {

        for (int bin_index = 1; bin_index <= nbins; bin_index++) {
            /// path to the root file
            TString rootfile = simulation_folder + eft_term + "/bin_" + to_string(bin_index) + "/Events/run_01/delphes_events_final.root";
            /// path to the banner that stores the cross-section
            string bannerfile = simulation_folder + eft_term + "/bin_" + to_string(bin_index) + "/Events/run_01/run_01_tag_1_banner.txt";

            cout << "Analysing file " << rootfile << endl;

            /// adding the file to the event loop run
            event_loop.addFile(rootfile);

            /// copying the distribution
            shared_ptr<Distribution> current_dist_ptr = invm_dist.clone();

            /// run the event analysis
            event_loop.run(&cms_analysis, current_dist_ptr);
            /// reading the cross-section
            double xsection = read_weight(bannerfile);       
            cout << "Cross-section: " << xsection << endl;

            /// rescaling the result by the weight
            /// number of events is not the same for all samples
            current_dist_ptr->rescaleDist(xsection * 1000. * 137. / 50000.);

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