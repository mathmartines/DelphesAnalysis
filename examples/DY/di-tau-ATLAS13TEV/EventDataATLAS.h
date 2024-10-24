#ifndef EVENT_DATA_ATLAS_H
#define EVENT_DATA_ATLAS_H

#include "DelphesAnalysis/EventData.h"

class EventDataATLAS: public EventData {

    public:
        /// @brief - set the tree reader pointer to extract the data from
        void setTree (std::shared_ptr<ExRootTreeReader> tree) override;
        


};



#endif