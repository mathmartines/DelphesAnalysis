#ifndef ANALYSIS_ATLAS
#define ANALYSIS_ATLAS

#include "TString.h"
#include "DelphesAnalysis/Analysis.h"
#include "DelphesAnalysis/EventData.h"
#include "SelectionsATLAS-ditau.h"

/// For all the ATLAS ditau analysis the particle selections are the same,
/// the only change comes from the cuts that defines the event selections
class ATLAS_ditau_search: public Analysis {
    public:
        ATLAS_ditau_search(): selections({&electron_selection, &muon_selection, &jets_requirements, &hadronic_taus_selection}){};        
        EventData* getEventData() override {return &event_data;};
        const ObjectSelection* getObjectSelection() const override {return &selections;};

    protected:
        /// default data that must be store for each event (Jet, Electron, etc)
        EventData event_data;
        /// Particle selections
        const ElectronCandidates electron_selection;
        const MuonCandidates muon_selection;
        const Jets jets_requirements;
        const HadronicTaus hadronic_taus_selection;
        /// @brief - holds all the selections above
        const CompositeSelections selections;
};

class ATLAS_ditau_had_bveto: public ATLAS_ditau_search {
    public:
        ATLAS_ditau_had_bveto(): ATLAS_ditau_search(), cuts({&leptons_veto, &tau_had_cuts, &b_veto}){
            analysis_name = "ATLAS di-hadronic tau search b-veto";
        };
        const Cut* getCuts() const override {return &cuts;};

    private:
        /// @brief - all cuts
        const LeptonsVeto leptons_veto;
        const HadronicTausCut tau_had_cuts;
        const bVeto b_veto;    
         /// @brief - holds all the cuts for the analysis
        const AnalysisCuts cuts;  
};


class ATLAS_ditau_had_btag: public ATLAS_ditau_search {
    public:
        ATLAS_ditau_had_btag(): ATLAS_ditau_search(), cuts({&leptons_veto, &tau_had_cuts, &btag}){
            analysis_name = "ATLAS di-hadronic tau search b-tagged";
        };
        const Cut* getCuts() const override {return &cuts;};

    private:
        /// @brief - all cuts
        const LeptonsVeto leptons_veto;
        const HadronicTausCut tau_had_cuts;
        const bTag btag;
         /// @brief - holds all the cuts for the analysis
        const AnalysisCuts cuts;
};

class ATLAS_taulep_tauhad_bveto: public ATLAS_ditau_search {
    public:
        ATLAS_taulep_tauhad_bveto(): ATLAS_ditau_search(), cuts({&single_lepton_cut, &lepton_pt_cut, &number_of_tauhad, &taulep_cuts, &b_veto}) {
            analysis_name = "ATLAS tau-lepton tau-had search b-veto";
        };
        const Cut* getCuts() const override {return &cuts;};

    private:
        /// @brief - all cuts
        const SingleLeptonCut single_lepton_cut;
        const LeptonPtCut lepton_pt_cut;
        const ContainsHadronicTaus number_of_tauhad;
        const TauLeptonEventsCuts taulep_cuts;
        const bVeto b_veto;
         /// @brief - holds all the cuts for the analysis
        const AnalysisCuts cuts;
};

class ATLAS_taulep_tauhad_btag: public ATLAS_ditau_search {
    public:
        ATLAS_taulep_tauhad_btag(): ATLAS_ditau_search(), cuts({&single_lepton_cut, &lepton_pt_cut, &number_of_tauhad, &taulep_cuts, &b_tag}) {
            analysis_name = "ATLAS tau-lepton tau-had search b-tag";
        };
        const Cut* getCuts() const override {return &cuts;};

    private:
        /// @brief - all cuts
        const SingleLeptonCut single_lepton_cut;
        const LeptonPtCut lepton_pt_cut;
        const ContainsHadronicTaus number_of_tauhad;
        const TauLeptonEventsCuts taulep_cuts;
        const bTag b_tag;
         /// @brief - holds all the cuts for the analysis
        const AnalysisCuts cuts;
};


#endif