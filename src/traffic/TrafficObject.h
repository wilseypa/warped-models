#ifndef TRAFFIC_OBJECT_H
#define TRAFFIC_OBJECT_H

#include <warped.h>
#include <SimulationObject.h>

#define MAX_CARS_ON_ROAD 5


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

    void initialize() {
        /* Create and send the initial events */
        /*IntVTime curr_time = static_cast<const IntVTime&> (getSimulationTime());
        car_direction_t car_arrival = (car_direction_t) 0;
        car_direction_t car_current_lane = car_arrival;
        TrafficEvent *event = new TrafficEvent( current_time,
                                                curr_time + (int) mean_interval_,
                                                this, this, 0, 0, 
                                                car_arrival, current_lane_, 
                                                ARRIVAL );
        this->receiveEvent(event);*/
    }

    void finalize() {}

    void executeProcess() {
    
        TrafficObjectState *my_state = static_cast<TrafficObjectState*>(getState());
        IntVTime current_time = dynamic_cast<const IntVTime&> (getSimulationTime());
        TrafficEvent *traffic_event = NULL;
        SimulationObject *receiver = NULL;

        while (haveMoreEvents()) {
            traffic_event = (TrafficEvent *) getEvent();
            if (!traffic_event) continue;

            switch (traffic_event->event_type_) {

                case ARRIVAL: {

                    if (!traffic_event->x_to_go_ && !traffic_event->y_to_go_) {
                        my_state->total_cars_finished_++;
                        break;
                    }
                    car_direction_t arrival_from = traffic_event->current_lane_;
                    my_state->total_cars_arrived_++;

                    switch (traffic_event->current_lane_) {

                        case WEST_LEFT: {
                            my_state->num_in_east_left_++;
                            arrival_from = EAST_LEFT;
                        } break;

                        case WEST_STRAIGHT: {
                            my_state->num_in_east_straight_++;
                            arrival_from = EAST_STRAIGHT;
                        } break;

                        case WEST_RIGHT: {
                            my_state->num_in_east_right_++;
                            arrival_from = EAST_RIGHT;
                        } break;

                        case EAST_LEFT: {
                            my_state->num_in_west_left_++;
                            arrival_from = WEST_LEFT;
                        } break;

                        case EAST_STRAIGHT: {
                            my_state->num_in_west_straight_++;
                            arrival_from = WEST_STRAIGHT;
                        } break;

                        case EAST_RIGHT: {
                            my_state->num_in_west_right_++;
                            arrival_from = WEST_RIGHT;
                        } break;

                        case NORTH_LEFT: {
                            my_state->num_in_south_left_++;
                            arrival_from = SOUTH_LEFT;
                        } break;

                        case NORTH_STRAIGHT: {
                            my_state->num_in_south_straight_++;
                            arrival_from = SOUTH_STRAIGHT;
                        } break;

                        case NORTH_RIGHT: {
                            my_state->num_in_south_right_++;
                            arrival_from = SOUTH_RIGHT;
                        } break;

                        case SOUTH_LEFT: {
                            my_state->num_in_north_left_++;
                            arrival_from = NORTH_LEFT;
                        } break;

                        case SOUTH_STRAIGHT: {
                            my_state->num_in_north_straight_++;
                            arrival_from = NORTH_STRAIGHT;
                        } break;

                        case SOUTH_RIGHT: {
                            my_state->num_in_north_right_++;
                            arrival_from = NORTH_RIGHT;
                        } break;
                    }

                    /*auto timestamp = traffic_event->ts_ + (unsigned int) std::ceil(interval_expo(*this->rng_));
                    events.emplace_back(new TrafficEvent {
                            this->name_, DIRECTION_SELECT, 
                            traffic_event->x_to_go_, traffic_event->y_to_go_, 
                            traffic_event->arrived_from_, arrival_from, timestamp});*/
                } break;

                case DEPARTURE: {

                    direction_t departure_direction = NORTH;

                    switch (traffic_event->current_lane_) {
                        case WEST_LEFT: {
                            my_state->num_out_west_left_--;
                            departure_direction = WEST;
                        } break;

                        case WEST_STRAIGHT: {
                            my_state->num_out_west_straight_--;
                            departure_direction = WEST;
                        } break;

                        case WEST_RIGHT: {
                            my_state->num_out_west_right_--;
                            departure_direction = WEST;
                        } break;

                        case EAST_LEFT: {
                            my_state->num_out_east_left_--;
                            departure_direction = EAST;
                        } break;

                        case EAST_STRAIGHT: {
                            my_state->num_out_east_straight_--;
                            departure_direction = EAST;
                        } break;

                        case EAST_RIGHT: {
                            my_state->num_out_east_right_--;
                            departure_direction = EAST;
                        } break;

                        case NORTH_LEFT: {
                            my_state->num_out_north_left_--;
                            departure_direction = NORTH;
                        } break;

                        case NORTH_STRAIGHT: {
                            my_state->num_out_north_straight_--;
                            departure_direction = NORTH;
                        } break;

                        case NORTH_RIGHT: {
                            my_state->num_out_north_right_--;
                            departure_direction = NORTH;
                        } break;

                        case SOUTH_LEFT: {
                            my_state->num_out_south_left_--;
                            departure_direction = SOUTH;
                        } break;

                        case SOUTH_STRAIGHT: {
                            my_state->num_out_south_straight_--;
                            departure_direction = SOUTH;
                        } break;

                        case SOUTH_RIGHT: {
                            my_state->num_out_south_right_--;
                            departure_direction = SOUTH;
                        } break;
                    }

                    /*auto timestamp = traffic_event->ts_ + (unsigned int) std::ceil(interval_expo(*this->rng_));
                    events.emplace_back(new TrafficEvent {
                            this->compute_move(departure_direction), ARRIVAL, 
                            traffic_event->x_to_go_, traffic_event->y_to_go_, 
                            traffic_event->arrived_from_, traffic_event->current_lane_, timestamp});*/
                } break;

                case DIRECTION_SELECT: {

                    int x_to_go = traffic_event->x_to_go_;
                    int y_to_go = traffic_event->y_to_go_;
                    car_direction_t current_lane = traffic_event->current_lane_;

                    switch (traffic_event->current_lane_) {
                        case EAST_LEFT: {
                            my_state->num_in_east_left_--;
                            if ((y_to_go < 0) && 
                                        (my_state->num_out_south_straight_ < MAX_CARS_ON_ROAD)) {
                                current_lane = SOUTH_STRAIGHT;
                                my_state->num_out_south_straight_++;
                                y_to_go++;

                            } else if ((x_to_go < 0) && 
                                        (my_state->num_out_south_right_ < MAX_CARS_ON_ROAD)){
                                current_lane = SOUTH_RIGHT;
                                my_state->num_out_south_right_++;
                                x_to_go++;

                            } else if ((x_to_go > 0) && 
                                        (my_state->num_out_south_left_ < MAX_CARS_ON_ROAD)){
                                current_lane = SOUTH_LEFT;
                                my_state->num_out_south_left_++;
                                x_to_go--;

                            } else {
                                if (traffic_event->arrived_from_ == SOUTH_LEFT) {
                                    current_lane = EAST_RIGHT;
                                    my_state->num_out_east_right_++;

                                } else if (traffic_event->arrived_from_ == EAST_STRAIGHT) {
                                    current_lane = EAST_STRAIGHT;
                                    my_state->num_out_east_straight_++;

                                } else if (traffic_event->arrived_from_ == NORTH_RIGHT) {
                                    current_lane = EAST_LEFT;
                                    my_state->num_out_east_left_++;
                                }
                            }
                        } break;

                        case EAST_STRAIGHT: {
                            my_state->num_in_east_straight_--;
                            if ((x_to_go < 0) && 
                                    (my_state->num_out_west_straight_ < MAX_CARS_ON_ROAD)) {
                                current_lane = WEST_STRAIGHT;
                                my_state->num_out_west_straight_++;
                                x_to_go++;

                            } else if ((y_to_go < 0) && 
                                    (my_state->num_out_west_left_ < MAX_CARS_ON_ROAD)) {
                                current_lane = WEST_LEFT;
                                my_state->num_out_west_left_++;
                                y_to_go++;

                            } else if ((y_to_go > 0) && 
                                    (my_state->num_out_west_right_ < MAX_CARS_ON_ROAD)) {
                                current_lane = WEST_RIGHT;
                                my_state->num_out_west_right_++;
                                y_to_go--;

                            } else {
                                if (traffic_event->arrived_from_ == NORTH_RIGHT) {
                                    current_lane = EAST_LEFT;
                                    my_state->num_out_east_left_++;

                                } else if (traffic_event->arrived_from_ == EAST_STRAIGHT) {
                                    current_lane = EAST_STRAIGHT;
                                    my_state->num_out_east_straight_++;

                                } else if (traffic_event->arrived_from_ == SOUTH_LEFT) {
                                    current_lane = EAST_RIGHT;
                                    my_state->num_out_east_right_++;
                                }
                            }
                        } break;

                        case EAST_RIGHT: {
                            my_state->num_in_east_right_--;
                            if ((y_to_go > 0) && 
                                    (my_state->num_out_north_straight_ < MAX_CARS_ON_ROAD)) {
                                current_lane = NORTH_STRAIGHT;
                                my_state->num_out_north_straight_++;
                                y_to_go--;

                            } else if ((x_to_go > 0) && 
                                    (my_state->num_out_north_right_ < MAX_CARS_ON_ROAD)) {
                                current_lane = NORTH_RIGHT;
                                my_state->num_out_north_right_++;
                                x_to_go --;

                            } else if ((x_to_go < 0) && 
                                    (my_state->num_out_north_left_ < MAX_CARS_ON_ROAD)) {
                                current_lane = NORTH_LEFT;
                                my_state->num_out_north_left_++;
                                x_to_go++;

                            } else {
                                if (traffic_event->arrived_from_ == SOUTH_LEFT) {
                                    current_lane = EAST_RIGHT;
                                    my_state->num_out_east_right_++;

                                } else if (traffic_event->arrived_from_ == EAST_STRAIGHT) {
                                    current_lane = EAST_STRAIGHT;
                                    my_state->num_out_east_straight_++;

                                } else if (traffic_event->arrived_from_ == NORTH_RIGHT) {
                                    current_lane = EAST_LEFT;
                                    my_state->num_out_east_left_++;
                                }
                            }
                        } break;

                        case WEST_LEFT: {
                            my_state->num_in_west_left_--;
                            if ((y_to_go > 0) && 
                                    (my_state->num_out_north_straight_ < MAX_CARS_ON_ROAD)) {
                                current_lane = NORTH_STRAIGHT;
                                my_state->num_out_north_straight_++;
                                y_to_go--;

                            } else if ((x_to_go > 0) && 
                                    (my_state->num_out_north_right_ < MAX_CARS_ON_ROAD)) {
                                current_lane = NORTH_RIGHT;
                                my_state->num_out_north_right_++;
                                x_to_go--;

                            } else if ((x_to_go < 0) && 
                                    (my_state->num_out_north_left_ < MAX_CARS_ON_ROAD)) {
                                current_lane = NORTH_LEFT;
                                my_state->num_out_north_left_++;
                                x_to_go++;

                            } else {
                                if (traffic_event->arrived_from_ == SOUTH_RIGHT) {
                                    current_lane = WEST_LEFT;
                                    my_state->num_out_west_left_++;

                                } else if (traffic_event->arrived_from_ == WEST_STRAIGHT) {
                                    current_lane = WEST_STRAIGHT;
                                    my_state->num_out_west_straight_++;

                                } else if (traffic_event->arrived_from_ == NORTH_LEFT) {
                                    current_lane = WEST_RIGHT;
                                    my_state->num_out_west_right_++;
                                }
                            }
                        } break;

                        case WEST_STRAIGHT: {
                            my_state->num_in_west_straight_--;
                            if ((x_to_go > 0) && 
                                    (my_state->num_out_east_straight_ < MAX_CARS_ON_ROAD)) {
                                current_lane = EAST_STRAIGHT;
                                my_state->num_out_east_straight_++;
                                x_to_go--;

                            } else if ((y_to_go > 0) && 
                                    (my_state->num_out_east_left_ < MAX_CARS_ON_ROAD)) {
                                current_lane = EAST_LEFT;
                                my_state->num_out_east_left_++;
                                y_to_go --;

                            } else if ((y_to_go < 0) && 
                                    (my_state->num_out_east_right_ < MAX_CARS_ON_ROAD)) {
                                current_lane = EAST_RIGHT;
                                my_state->num_out_east_right_++;
                                y_to_go++;

                            } else {
                                if (traffic_event->arrived_from_ == SOUTH_RIGHT) {
                                    current_lane = WEST_LEFT;
                                    my_state->num_out_west_left_++;

                                } else if (traffic_event->arrived_from_ == WEST_STRAIGHT) {
                                    current_lane = WEST_STRAIGHT;
                                    my_state->num_out_west_straight_++;

                                } else if (traffic_event->arrived_from_ == NORTH_LEFT) {
                                    current_lane = WEST_RIGHT;
                                    my_state->num_out_west_right_++;
                                }
                            }
                        } break;

                        case WEST_RIGHT: {
                            my_state->num_in_west_right_--;
                            if ((y_to_go < 0) && 
                                    (my_state->num_out_south_straight_ < MAX_CARS_ON_ROAD)) {
                                current_lane = SOUTH_STRAIGHT;
                                my_state->num_out_south_straight_++;
                                y_to_go++;

                            } else if ((x_to_go > 0) && 
                                    (my_state->num_out_south_left_ < MAX_CARS_ON_ROAD)) {
                                current_lane = SOUTH_LEFT;
                                my_state->num_out_south_left_++;
                                x_to_go--;

                            } else if ((x_to_go < 0) && 
                                    (my_state->num_out_south_right_ < MAX_CARS_ON_ROAD)) {
                                current_lane = SOUTH_RIGHT;
                                my_state->num_out_south_right_++;
                                x_to_go++;

                            } else {
                                if (traffic_event->arrived_from_ == SOUTH_RIGHT) {
                                    current_lane = WEST_LEFT;
                                    my_state->num_out_west_left_++;

                                } else if (traffic_event->arrived_from_ == WEST_STRAIGHT) {
                                    current_lane = WEST_STRAIGHT;
                                    my_state->num_out_west_straight_++;

                                } else if (traffic_event->arrived_from_ == NORTH_LEFT) {
                                    current_lane = WEST_RIGHT;
                                    my_state->num_out_west_right_++;
                                }
                            }
                        } break;

                        case NORTH_LEFT: {
                            my_state->num_in_north_left_--;
                            if ((x_to_go > 0) && 
                                    (my_state->num_out_east_straight_ < MAX_CARS_ON_ROAD)) {
                                current_lane = EAST_STRAIGHT;
                                my_state->num_out_east_straight_++;
                                x_to_go--;

                            } else if ((y_to_go > 0) && 
                                    (my_state->num_out_east_left_ < MAX_CARS_ON_ROAD)) {
                                current_lane = EAST_LEFT;
                                my_state->num_out_east_left_++;
                                y_to_go--;

                            } else if ((y_to_go < 0) && 
                                    (my_state->num_out_east_right_ < MAX_CARS_ON_ROAD)) {
                                current_lane = EAST_RIGHT;
                                my_state->num_out_east_right_++;
                                y_to_go++;

                            } else {
                                if (traffic_event->arrived_from_ == WEST_RIGHT) {
                                    current_lane = NORTH_LEFT;
                                    my_state->num_out_north_left_++;

                                } else if (traffic_event->arrived_from_ == NORTH_STRAIGHT) {
                                    current_lane = NORTH_STRAIGHT;
                                    my_state->num_out_north_straight_++;

                                } else if (traffic_event->arrived_from_ == EAST_LEFT) {
                                    current_lane = NORTH_RIGHT;
                                    my_state->num_out_north_right_++;
                                }
                            }
                        } break;

                        case NORTH_STRAIGHT: {
                            my_state->num_in_north_straight_--;
                            if ((y_to_go < 0) && 
                                    (my_state->num_out_south_straight_ < MAX_CARS_ON_ROAD)) {
                                current_lane = SOUTH_STRAIGHT;
                                my_state->num_out_south_straight_++;
                                y_to_go++;

                            } else if ((x_to_go > 0) && 
                                    (my_state->num_out_south_left_ < MAX_CARS_ON_ROAD)) {
                                current_lane = SOUTH_LEFT;
                                my_state->num_out_south_left_++;
                                x_to_go--;

                            } else if ((x_to_go < 0) && 
                                    (my_state->num_out_south_right_ < MAX_CARS_ON_ROAD)) {
                                current_lane = SOUTH_RIGHT;
                                my_state->num_out_south_right_++;
                                x_to_go++;

                            } else {
                                if (traffic_event->arrived_from_ == WEST_RIGHT) {
                                    current_lane = NORTH_LEFT;
                                    my_state->num_out_north_left_++;

                                } else if (traffic_event->arrived_from_ == NORTH_STRAIGHT) {
                                    current_lane = NORTH_STRAIGHT;
                                    my_state->num_out_north_straight_++;

                                } else if (traffic_event->arrived_from_ == EAST_LEFT) {
                                    current_lane = NORTH_RIGHT;
                                    my_state->num_out_north_right_++;
                                }
                            }
                        } break;

                        case NORTH_RIGHT: {
                            my_state->num_in_north_right_--;
                            if ((x_to_go < 0) && 
                                    (my_state->num_out_west_straight_ < MAX_CARS_ON_ROAD)) {
                                current_lane = WEST_STRAIGHT;
                                my_state->num_out_west_straight_++;
                                x_to_go++;

                            } else if ((y_to_go < 0) && 
                                    (my_state->num_out_west_left_ < MAX_CARS_ON_ROAD)) {
                                current_lane = WEST_LEFT;
                                my_state->num_out_west_left_++;
                                y_to_go++;

                            } else if ((y_to_go > 0) && 
                                    (my_state->num_out_west_right_ < MAX_CARS_ON_ROAD)) {
                                current_lane = WEST_RIGHT;
                                my_state->num_out_west_right_++;
                                y_to_go--;

                            } else {
                                if (traffic_event->arrived_from_ == WEST_RIGHT) {
                                    current_lane = NORTH_LEFT;
                                    my_state->num_out_north_left_++;

                                } else if (traffic_event->arrived_from_ == NORTH_STRAIGHT) {
                                    current_lane = NORTH_STRAIGHT;
                                    my_state->num_out_north_straight_++;

                                } else if (traffic_event->arrived_from_ == EAST_LEFT) {
                                    current_lane = NORTH_RIGHT;
                                    my_state->num_out_north_right_++;
                                }
                            }
                        } break;

                        case SOUTH_LEFT: {
                            my_state->num_in_south_left_--;
                            if ((x_to_go < 0) && 
                                    (my_state->num_out_west_straight_ < MAX_CARS_ON_ROAD)) {
                                current_lane = WEST_STRAIGHT;
                                my_state->num_out_west_straight_++;
                                x_to_go++;

                            } else if ((y_to_go < 0) && 
                                    (my_state->num_out_west_left_ < MAX_CARS_ON_ROAD)) {
                                current_lane = WEST_LEFT;
                                my_state->num_out_west_left_++;
                                y_to_go++;

                            } else if ((y_to_go > 0) && 
                                    (my_state->num_out_west_right_ < MAX_CARS_ON_ROAD)) {
                                current_lane = WEST_RIGHT;
                                my_state->num_out_west_right_++;
                                y_to_go--;

                            } else {
                                if (traffic_event->arrived_from_ == WEST_LEFT) {
                                    current_lane = SOUTH_RIGHT;
                                    my_state->num_out_south_right_++;

                                } else if (traffic_event->arrived_from_ == SOUTH_STRAIGHT) {
                                    current_lane = SOUTH_STRAIGHT;
                                    my_state->num_out_south_straight_++;

                                } else if (traffic_event->arrived_from_ == EAST_RIGHT) {
                                    current_lane = SOUTH_LEFT;
                                    my_state->num_out_south_left_++;
                                }
                            }
                        } break;

                        case SOUTH_STRAIGHT: {
                            my_state->num_in_south_straight_--;
                            if ((y_to_go > 0) && 
                                    (my_state->num_out_north_straight_ < MAX_CARS_ON_ROAD)) {
                                current_lane = NORTH_STRAIGHT;
                                my_state->num_out_north_straight_++;
                                y_to_go--;

                            } else if ((x_to_go < 0) && 
                                    (my_state->num_out_north_left_ < MAX_CARS_ON_ROAD)) {
                                current_lane = NORTH_LEFT;
                                my_state->num_out_north_left_++;
                                x_to_go++;

                            } else if ((x_to_go > 0) && 
                                    (my_state->num_out_north_right_ < MAX_CARS_ON_ROAD)) {
                                current_lane = NORTH_RIGHT;
                                my_state->num_out_north_right_++;
                                x_to_go --;

                            } else {
                                if (traffic_event->arrived_from_ == EAST_RIGHT) {
                                    current_lane = SOUTH_LEFT;
                                    my_state->num_out_south_left_++;

                                } else if (traffic_event->arrived_from_ == SOUTH_STRAIGHT) {
                                    current_lane = SOUTH_STRAIGHT;
                                    my_state->num_out_south_straight_++;

                                } else if (traffic_event->arrived_from_ == WEST_LEFT) {
                                    current_lane = SOUTH_RIGHT;
                                    my_state->num_out_south_right_++;
                                }
                            }
                        } break;

                        case SOUTH_RIGHT: {
                            my_state->num_in_south_right_--;
                            if ((x_to_go > 0) && 
                                    (my_state->num_out_east_straight_ < MAX_CARS_ON_ROAD)) {
                                current_lane = EAST_STRAIGHT;
                                my_state->num_out_east_straight_++;
                                x_to_go--;

                            } else if ((y_to_go > 0) && 
                                    (my_state->num_out_east_left_ < MAX_CARS_ON_ROAD)) {
                                current_lane = EAST_LEFT;
                                my_state->num_out_east_left_++;
                                y_to_go--;

                            } else if ((y_to_go < 0) && 
                                    (my_state->num_out_east_right_ < MAX_CARS_ON_ROAD)) {
                                current_lane = EAST_RIGHT;
                                my_state->num_out_east_right_++;
                                y_to_go++;

                            } else {
                                if (traffic_event->arrived_from_ == EAST_RIGHT) {
                                    current_lane = SOUTH_LEFT;
                                    my_state->num_out_south_left_++;

                                } else if (traffic_event->arrived_from_ == SOUTH_STRAIGHT) {
                                    current_lane = SOUTH_STRAIGHT;
                                    my_state->num_out_south_straight_++;

                                } else if (traffic_event->arrived_from_ == WEST_LEFT) {
                                    current_lane = SOUTH_RIGHT;
                                    my_state->num_out_south_right_++;
                                }
                            }
                        } break;
                    }

                    /*auto timestamp = traffic_event->ts_ + (unsigned int) std::ceil(interval_expo(*this->rng_));
                    events.emplace_back(new TrafficEvent {
                            this->name_, DEPARTURE, x_to_go, y_to_go, 
                            traffic_event->current_lane_, current_lane, timestamp});*/
                } break;

                default: {
                    assert(0);
                }
            }
        }
    }

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

    /*std::string compute_move(direction_t direction) {
    }*/
};

#endif
