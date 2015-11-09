#ifndef TRAFFIC_OBJECT_H
#define TRAFFIC_OBJECT_H

#include <warped.h>
#include <SimulationObject.h>

/* This class is the simulation object for the Traffic simulation. */

enum direction_t {

    NORTH, 
    SOUTH, 
    EAST, 
    WEST
};

class TrafficObject : public SimulationObject {
  
public :
    TrafficObject(  const unsigned int num_intersections_x,
                    const unsigned int num_intersections_y,
                    const unsigned int num_cars,
                    const unsigned int mean_interval,
                    const unsigned int index) :
            obj_name_("Intersection" + std::to_string(index)),
            num_intersections_x_(num_intersections_x),
            num_intersections_y_(num_intersections_y),
            num_cars_(num_cars),
            mean_interval_(mean_interval),
            index_(index) {}


    ~TrafficObject() {}

    void initialize() {}

    void finalize() {}

    void executeProcess() {}

    State *allocateState() {
        return new TrafficObjectState();
    }

    void deallocateState( const State* state ) {
        delete state;
    }

    void reclaimEvent( const Event *event ) {
        delete event;
    }

    const std::string &getName() const {
        return obj_name_;
    }

private :
    const std::string obj_name_;
    const unsigned int num_intersections_x_;
    const unsigned int num_intersections_y_;
    const unsigned int num_cars_;
    const unsigned int mean_interval_;
    const unsigned int index_;

    std::string compute_move(direction_t direction) {
    }
};

#endif
