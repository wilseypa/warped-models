#ifndef TRAFFIC_APPLICATION_H
#define TRAFFIC_APPLICATION_H

#include <vector>
#include <Application.h>
#include "TrafficObject.h"

class TrafficApplication : public Application {

public:
    TrafficApplication( unsigned int num_intersections_x,
                        unsigned int num_intersections_y,
                        unsigned int num_cars,
                        unsigned int mean_interval ) :
                num_x_(num_intersections_x),
                num_y_(num_intersections_y),
                num_cars_(num_cars),
                mean_interval_(mean_interval) {}

    std::vector<SimulationObject *> *getSimulationObjects() {

        std::vector<SimulationObject *> *retval = new std::vector<SimulationObject *>;
        for (unsigned int i = 0; i < num_x_*num_y_; i++) {
            retval->push_back( 
                    new TrafficObject(num_x_, num_y_, num_cars_, mean_interval_, i) );
        }
        return retval;
    }

    void registerDeserializers() {
        DeserializerManager::instance()->registerDeserializer(
                                            TrafficEvent::getTrafficEventDataType(), 
                                            &TrafficEvent::deserialize);
    }


private:
    unsigned int num_x_;
    unsigned int num_y_;
    unsigned int num_cars_;
    unsigned int mean_interval_;
};

#endif
