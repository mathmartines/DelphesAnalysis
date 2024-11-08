#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <iostream>
#include <memory>
#include <vector>
#include "DelphesAnalysis/EventData.h"
#include "DelphesAnalysis/Observable.h"


/// @brief - Interface for all possible distributions
class Distribution {
    public:
        /// @brief - updates the distribution with the given event 
        virtual void updateDistribution(const EventData* event_data) = 0;
        
        /// @brief - clones the distribution with all the bins set to zero
        virtual std::shared_ptr<Distribution> clone() const = 0;

        /// @brief - rescale the distribution by a common factor
        virtual void rescaleDist(const double weight) = 0;

        /// @brief - show number of events
        virtual void displayNumberOfEvents() const = 0;

};

class ObservableDistribution: public Distribution {
    public:
        ObservableDistribution(std::vector<double> bin_edges, EventObservable* obs);

        /// @brief - updates the distribution with the given event 
        void updateDistribution(const EventData* event_data) override;
        
        /// @brief - clones the distribution with all the bins set to zero
        std::shared_ptr<Distribution> clone() const override;

        /// @brief - rescale the distribution by a common factor
        void rescaleDist(const double weight) override;

        /// @brief - display the number of events in the distribution
        void displayNumberOfEvents() const override;

        /// @brief - set all bin values to zero
        void clear ();
        
        /// @brief - returns the vector that holds the dist 
        const std::vector<double>& getBinsContent() const {return number_of_evts;};

        /// @brief - acess bin contents
        double& operator[](const int index) {return number_of_evts[index];};
        const double& operator[](const int index) const {return number_of_evts[index];};

        /// @brief - sums the bin content
        ObservableDistribution& operator+=(const ObservableDistribution& dist);
        ObservableDistribution operator+ (const ObservableDistribution& dist);
    
    private:
        std::vector<double> number_of_evts; /// total number of events 
        std::vector<double> edges; /// specify the bins of the distribution
        EventObservable* observable; /// stores how the observable is calculated

        /// @brief - finds the indice of the bin to be updated 
        ///        - returns the bin index or -1, which means that the value is outside of the distribution limit
        int findBinIndex(double observable_value) const;
};


#endif

