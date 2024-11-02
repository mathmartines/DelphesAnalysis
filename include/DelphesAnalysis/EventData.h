#ifndef EVENTDATA_H
#define EVENTDATA_H

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <variant>
#include "TClonesArray.h"
#include "TString.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "classes/DelphesClasses.h"

/**
 * @class - generic container to store the data needed for the analysis of each event
 **/
class EventData { 

    public:
        using Particles = std::variant<Electron*, Muon*, Jet*>;
        using ClonesArrayMap = std::map<TString, const TClonesArray*>;
        using ParticlesVector = std::vector<Particles>;
        using ParticlesVectorMap = std::map<TString, ParticlesVector>;

        /// @brief - set the tree reader pointer to extract the data from
        void setTree (std::shared_ptr<ExRootTreeReader> tree);

        /// @brief - tells the object which information we need to store from the event. Defines the branches used 
        ///          in the analysis.
        virtual void setBranches() = 0;

        /// @brief - returns a reference to the branch
        const TClonesArray* const &getBranch (TString branchname) const;

        /// @brief - returns a reference to the particles vectors
        std::vector<Particles> &getParticles (TString particles_name);
        const std::vector<Particles> &getParticles (TString particles_name) const;

        /// @brief - resets the particles vector
        void resetVectors ();
        
        /// @brief - returns a vector with all the particles
        ///          this returns a copy of the vector, all the modifications made are not saved by the object
        template<typename T>
        std::vector<T*> getTypedParticles (TString particles_name);

        ///
        template<typename T>
        static T* getPtrToParticle(Particles& particle);

    protected:
        /// @brief - pointer to the tree object
        std::shared_ptr<ExRootTreeReader> tree_reader;

        /// @brief - stores all the branches for the analysis
        ClonesArrayMap branches;    

        /// @brief - stores the particles that will be used in the analysis
        ParticlesVectorMap candidates;

        /// @brief - adds a new branch to the event data
        void addBranch(TString branch_name);

        /// @brief - defines a vector to store new particles
        void addParticlesVector(TString particles_name);
};

template<typename T>
T* EventData::getPtrToParticle(EventData::Particles& particle) {
    if (T* converted_particle = *std::get_if<T*>(&particle))
        return converted_particle;
    throw std::runtime_error("trying to extract particle from different type");
};

template<typename T>
std::vector<T*> EventData::getTypedParticles (TString particles_name) {
    std::vector<T*> particles;
    for (auto& particle: getParticles(particles_name)) {
        if (T* typed_particle = std::get_if<T*>(particle))
            particles.push_back(typed_particle);
    }
    return particles;
};


/**
 * @class - abstract class to store and access the data concerning the events
 */
class EventDataOld {
    public:
        /// @brief - set the tree reader p  ointer to extract the data from
        virtual void setTree (std::shared_ptr<ExRootTreeReader> tree);
        
        /// @brief - acess to the branch information
        const TClonesArray* const &getJets() const {return branch_jets;};
        const TClonesArray* const &getElectrons() const {return branch_electrons;};
        const TClonesArray* const &getMuons() const {return branch_muons;};
        const TClonesArray* const &getMET() const {return branch_met;};
        const TClonesArray* const &getTracks() const {return branch_track;};

        /// @brief - vectors to store the pointers to the particles we'll use in the analysis
        std::vector<Electron*> electrons;
        std::vector<Muon*> muons;
        std::vector<Jet*> jets;
        std::vector<Jet*> hadronic_taus;

    protected:
        /// @brief - pointer to the tree object
        std::shared_ptr<ExRootTreeReader> tree_reader;

        /// default branches
        const TClonesArray* branch_jets = nullptr;
        const TClonesArray* branch_electrons = nullptr;
        const TClonesArray* branch_muons = nullptr;
        const TClonesArray* branch_met = nullptr;
        const TClonesArray* branch_track = nullptr;
};

#endif