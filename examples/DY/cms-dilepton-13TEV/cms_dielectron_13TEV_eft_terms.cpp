#include <iostream>
#include <vector>
#include <memory>
#include "TString.h"
#include "DelphesAnalysis/EventLoop.h"
#include "DelphesAnalysis/EventData.h"
#include "DelphesAnalysis/EventAnalysis.h"
#include "DelphesAnalysis/Distribution.h"
#include "DelphesAnalysis/Observable.h"
#include "EventDataCMS_dilepton.h"
#include "SelectionsCMS_dilepton.h"


using namespace std;

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
    TString simulation_folder = "/home/martines/work/MG5_aMC_v3_1_1/PhD/DY/cms-dielectron-13TEV/UniversalSMEFT_d8/";
    /// all the masses
    vector<TString> eft_terms = {"SM"};
    /// number of bins in the folder 
    int nbins = 2;

    /// runs the analysis in all the simulations
    for (auto eft_term: eft_terms) {
        for (int bin_index = 1; bin_index <= nbins; bin_index++) {
            /// path to the root file
            TString rootfile = simulation_folder + "/" + eft_term + "/bin_" + to_string(bin_index) + "/Events/run_01/delphes.root";
            cout << "Analysing file " << rootfile << endl;
            /// adding the file to the event loop run
            event_loop.addFile(rootfile);
            /// copying the distribution
            shared_ptr<Distribution> current_dist_ptr = invm_dist.clone();
            /// run the event analysis
            event_loop.run(&cms_analysis, current_dist_ptr);
            /// cuecking the output 
            ObservableDistribution& current_dist = dynamic_cast<ObservableDistribution&>(*current_dist_ptr);
            const vector<double> nevts = current_dist.getBinsContent();
            for (auto val: nevts)
                cout << val << ", ";
            cout << "\n";
            /// adds the content to the final distribution
            invm_dist += current_dist;
            /// reset the object for the next analysis
            event_loop.reset();
        }
        for (auto val: invm_dist.getBinsContent())
            cout << val << ", ";
        
        cout << endl;
    }

    return 0;
}