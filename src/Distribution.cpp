#include "DelphesAnalysis/Distribution.h"

ObservableDistribution::ObservableDistribution(std::vector<double> bin_edges, const EventObservable* obs): 
    edges(bin_edges), 
    observable(obs),
    number_of_evts(bin_edges.size() - 1, 0) {}

int ObservableDistribution::findBinIndex(double observable_value) const {
    for (int bin_index = 0; bin_index < edges.size() - 1; bin_index++) {
        // checking if the value is inside the current bin
        if (observable_value >= edges[bin_index] && observable_value < edges[bin_index + 1])
            return bin_index;
    }
    /// outside of the distribution index
    return -1;
}

void ObservableDistribution::updateDistribution(const EventData* event_data) {
    // calculating the value of the observable 
    double obs_value = observable->evaluateObservable(event_data);
    /// find the bin index that we need to update 
    int bin_index = findBinIndex(obs_value);
    /// updates the distribution
    if (bin_index >= 0) number_of_evts[bin_index]++;
}

std::shared_ptr<Distribution> ObservableDistribution::clone() const {
    return std::make_shared<ObservableDistribution>(edges, observable);
} 

void ObservableDistribution::rescaleDist(const double weight) {
    for(int index = 0; index < number_of_evts.size(); index++)
        number_of_evts[index] *= weight;
}

void ObservableDistribution::clear() {
    for(int index = 0; index < number_of_evts.size(); index++)
        number_of_evts[index] = 0;
}

ObservableDistribution& ObservableDistribution::operator+=(const ObservableDistribution& dist) {
    // checking if the number of bins is the same 
    if (number_of_evts.size() != dist.getBinsContent().size())
        return *this;

    // adding the entries    
    for (int index = 0; index < number_of_evts.size(); index++) 
        (*this)[index] += dist.getBinsContent()[index];

    return *this;
}

ObservableDistribution ObservableDistribution::operator+(const ObservableDistribution& dist) {
    ObservableDistribution summed_dist (edges, observable);
    for (int i = 0; i < number_of_evts.size(); i++) {
        summed_dist[i] = number_of_evts[i] + dist[i];
    }
    return summed_dist;
}

void ObservableDistribution::displayNumberOfEvents() const {
    std::cout << "Number of events in each bin: ";
    for (int i = 0; i < number_of_evts.size(); i++) {
        if (i < number_of_evts.size() - 1)
            std::cout << number_of_evts[i] << ", ";
        else 
            std::cout << number_of_evts[i];
    }
    std::cout << std::endl;
}