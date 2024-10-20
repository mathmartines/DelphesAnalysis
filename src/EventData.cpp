#include "DelphesAnalysis/EventData.h"

void EventData::setTree(std::shared_ptr<ExRootTreeReader> tree) {
    tree_reader = tree;
    /// link the branches ptrs to the tree
    branch_jets = tree_reader->UseBranch("Jet");
    branch_electrons = tree_reader->UseBranch("Electron");
    branch_muons = tree_reader->UseBranch("Muon");
    branch_met = tree_reader->UseBranch("MissingET");
}