#include "DelphesAnalysis/EventData.h"


void EventData::setTree(std::shared_ptr<ExRootTreeReader> tree) {
    tree_reader = tree;
    /// lets the subclass decide which data must be stored
    setBranches();
}

const TClonesArray* const &EventData::getBranch(TString branch_name) const {
    /// checks if we have a branch with this name has been defined
    auto search = branches.find(branch_name); 
    if (search == branches.end())
        throw std::runtime_error("Asking acess for a key not in the map");
    return search->second;
}

std::vector<EventData::Particles> &EventData::getParticles(TString particles_name) {
    /// checks if the particles are in the array
    auto search = candidates.find(particles_name);
    if (search == candidates.end())
        throw std::runtime_error("Asking acess for a key not in the map");
    return search->second;
} 


void EventData::addBranch(TString branch_name) {
    branches[branch_name] = tree_reader->UseBranch(branch_name);
}

void EventData::addParticlesVector(TString particles_name) {
    candidates[particles_name] = std::vector<EventData::Particles>();
}



void EventDataOld::setTree(std::shared_ptr<ExRootTreeReader> tree) {
    tree_reader = tree;
    /// link the branches ptrs to the tree
    branch_jets = tree_reader->UseBranch("Jet");
    branch_electrons = tree_reader->UseBranch("Electron");
    branch_muons = tree_reader->UseBranch("Muon");
    branch_met = tree_reader->UseBranch("MissingET");
    branch_track = tree_reader->UseBranch("Track");
}