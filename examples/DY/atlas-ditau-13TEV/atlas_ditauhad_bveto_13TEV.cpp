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

double correction_factor(string subfolder) {
    /// find the / caracther
    size_t pos = subfolder.find("/");
    /// eft term
    string eft_term = subfolder.substr(pos);
    /// returns the corrected factor
    if (eft_term.find("-") != string::npos)
        return 1.0e12;
    return 1.0e6;
}

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
    vector<double> bin_edges = {150., 200., 250., 300., 350., 400., 450., 500., 600., 700., 800., 900., 1000., 1150., 15000.};
    TransverseMassDiTauHad mt_atlas;
    shared_ptr<ObservableDistribution> transverse_mass_dist = make_shared<ObservableDistribution> (bin_edges, &mt_atlas);

    /// Folder where the simulations are stored
    string simulations_folders = "/data/01/martines/MG5_aMC_v3_1_1/PhD/High-PT/atlas-ditau-13TEV/";
    
    /// @todo - INCLUDE THE OTHER FOLDERS
    /// Maps with all the simulations root folders
    map<string, string> simulations {
        {"uubar_int-gamma", "uubar_int-gamma/C1lq"},
        {"uubar_reg-reg", "uubar_reg-reg/C1lq-C1lq"},
        {"bdbar_reg-reg", "bdbar_reg-reg/C1lq-C1lq"},
        {"ssbar_reg-reg", "ssbar_reg-reg/C1lq-C1lq"},
        {"sdbar_reg-reg", "sdbar_reg-reg/C1lq-C1lq"},
        {"sbbar_reg-reg", "sbbar_reg-reg/C1lq-C1lq"},
        {"bbbar_reg-reg", "bbbar_reg-reg/C1lq-C1lq"},
        {"uubar_int-Z", "uubar_int-Z/C1lq"},
        {"dbbar_reg-reg", "dbbar_reg-reg/C1lq-C1lq"},
        {"bbbar_int-Z", "bbbar_int-Z/C1lq"},
        {"ccbar_int-Z", "ccbar_int-Z/C1lq"},
        {"ddbar_int-gamma", "ddbar_int-gamma/C1lq"},
        {"ddbar_int-Z", "ddbar_int-Z/C1lq"},
        {"bsbar_reg-reg", "bsbar_reg-reg/C1lq-C1lq"},
        {"ccbar_reg-reg", "ccbar_reg-reg/C1lq-C1lq"},
        {"ccbar_int-gamma", "ccbar_int-gamma/C1lq"},
        {"ssbar_int-Z", "ssbar_int-Z/C1lq"},
        {"ddbar_reg-reg", "ddbar_reg-reg/C1lq-C1lq"},
        {"bbbar_int-gamma", "bbbar_int-gamma/C1lq"},
        {"ssbar_int-gamma", "ssbar_int-gamma/C1lq"},
        {"dsbar_reg-reg", "dsbar_reg-reg/C1lq-C1lq"},
        {"ucbar_reg-reg", "ucbar_reg-reg/C1lq-C1lq"},
        {"cubar_reg-reg", "cubar_reg-reg/C1lq-C1lq"}
    };

    /// @todo - CORRECT THE CROSS-SECTIONS - create a map for it or a function 

    /// Total number of bins for each simulation
    int nbins = 14;

    /// iterates over all the simulations
    for (const auto& [process, subfolder] : simulations) {
        // json file to store the output of the simulations
        json simulation_efficieny;     

        /// vector to store the efficiencies
        vector<vector<double>> eff;
    
        /// vector to store the cross-sections
        vector<double> xsecs;

        /// correction factor for the cross-section (I defined the coeff with value of 1e-6 which is not needed)
        double corr = correction_factor(subfolder);
        
        /// iterates over all the bins to get the efficiencies
        for (int bin_index = 1; bin_index <= nbins; bin_index++) {

            /// Path to the .root file 
            TString root_file = simulations_folders + subfolder + "/bin_" + to_string(bin_index) + "/Events/run_01/delphes_events.root";

            /// path to the banner that stores the cross-section
            string bannerfile = simulations_folders + subfolder + "/bin_" + to_string(bin_index) + "/Events/run_01/run_01_tag_1_banner.txt";

            cout << endl << "Analysing file " << root_file << endl;

            /// Adds the file to the event loop run
            event_loop.addFile(root_file);

            /// Launch the analysis - it returns the total number of evts in the file
            int number_of_evts = event_loop.run(&atlas_analysis, transverse_mass_dist);

            /// reading the cross-section
            double xsection = read_weight(bannerfile) * corr;    

            cout << "sigma (pb): " << xsection << endl;

            /// divide by the total number of events
            transverse_mass_dist->rescaleDist(1./number_of_evts);

            /// Adds the content of the distribution to the .json file
            eff.push_back(transverse_mass_dist->getBinsContent());
            xsecs.push_back(xsection);

            /// Reset for the next file
            event_loop.reset();

            /// Clear the distribution for the next simulation
            transverse_mass_dist->clear();
        }

        simulation_efficieny["efficiencies"] = eff;
        simulation_efficieny["cross-sections"] = xsecs;

        /// Saves .json file
        ofstream file("High-PT/" + process + ".json");
        if (file.is_open()) {
            file << simulation_efficieny.dump(2); 
            file.close();
            cout << "JSON file created successfully!" << std::endl;
        }
    }

    return 0;
}