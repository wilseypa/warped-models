#ifndef TRAFFIC_OBJECT_H
#define TRAFFIC_OBJECT_H

#include <warped.h>
#include <SimulationObject.h>
#include "IntVTime.h"
#include "TrafficObjectState.h"
#include "TrafficEvent.h"
#include "rnd/DiscUnif.h"
#include "rnd/NegExp.h"

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
            obj_name_(objectName(index)),
            num_intersections_x_(num_intersections_x),
            num_intersections_y_(num_intersections_y),
            num_cars_(num_cars),
            mean_interval_(mean_interval),
            index_(index) {}


    ~TrafficObject() {}

    void initialize() {

        TrafficObjectState *traffic_state = dynamic_cast<TrafficObjectState *>(getState());
        ASSERT(traffic_state);
        delete traffic_state->gen_;
        traffic_state->gen_ = new MLCG(index_, index_+1);

        for (unsigned int i = 0; i < num_cars_; i++) {

            NegativeExpntl interval_neg_expo(mean_interval_, traffic_state->gen_);
            int interval = (int) interval_neg_expo();

            DiscreteUniform rand_car_direction(0, 11, traffic_state->gen_);
            car_direction_t car_arrival = (car_direction_t) rand_car_direction();
            car_direction_t car_current_lane = car_arrival;

            DiscreteUniform rand_x(-99, 100, traffic_state->gen_);
            int x = (int) rand_x();

            DiscreteUniform rand_y(-99, 100, traffic_state->gen_);
            int y = (int) rand_y();

            IntVTime curr_time = static_cast<const IntVTime&> (getSimulationTime());
            TrafficEvent *event = new TrafficEvent( curr_time, curr_time + interval,
                                                    this, this, 
                                                    x, y, 
                                                    car_arrival, car_current_lane, 
                                                    ARRIVAL );
            this->receiveEvent(event);
        }
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

            switch (traffic_event->getEventType()) {

                case ARRIVAL: {

                    if (!traffic_event->getX() && !traffic_event->getY()) {
                        my_state->total_cars_finished_++;
                        break;
                    }
                    car_direction_t arrival_from = traffic_event->getCurrentLane();
                    my_state->total_cars_arrived_++;

                    switch (traffic_event->getCurrentLane()) {

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

                    IntVTime curr_time = static_cast<const IntVTime&> (getSimulationTime());
                    NegativeExpntl interval_neg_expo(mean_interval_, my_state->gen_);
                    int interval = (int) interval_neg_expo();
                    TrafficEvent *event = 
                        new TrafficEvent(   curr_time, curr_time + interval, this, this,
                                            traffic_event->getX(), traffic_event->getY(), 
                                            traffic_event->getArrivedFrom(), arrival_from, 
                                            DIRECTION_SELECT    );
                    this->receiveEvent(event);

                } break;

                case DEPARTURE: {

                    direction_t departure_direction = NORTH;

                    switch (traffic_event->getCurrentLane()) {
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

                    IntVTime curr_time = static_cast<const IntVTime&> (getSimulationTime());
                    NegativeExpntl interval_neg_expo(mean_interval_, my_state->gen_);
                    int interval = (int) interval_neg_expo();
                    TrafficEvent *event = 
                        new TrafficEvent(   curr_time, 
                                            curr_time + interval, 
                                            this, this, 
                                            //computeMove(departure_direction), 
                                            traffic_event->getX(), 
                                            traffic_event->getY(), 
                                            traffic_event->getArrivedFrom(), 
                                            traffic_event->getCurrentLane(), 
                                            ARRIVAL     );
                    this->receiveEvent(event);

                } break;

                case DIRECTION_SELECT: {

                    int x_to_go = traffic_event->getX();
                    int y_to_go = traffic_event->getY();
                    car_direction_t current_lane = traffic_event->getCurrentLane();

                    switch (traffic_event->getCurrentLane()) {
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
                                if (traffic_event->getArrivedFrom() == SOUTH_LEFT) {
                                    current_lane = EAST_RIGHT;
                                    my_state->num_out_east_right_++;

                                } else if (traffic_event->getArrivedFrom() == EAST_STRAIGHT) {
                                    current_lane = EAST_STRAIGHT;
                                    my_state->num_out_east_straight_++;

                                } else if (traffic_event->getArrivedFrom() == NORTH_RIGHT) {
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
                                if (traffic_event->getArrivedFrom() == NORTH_RIGHT) {
                                    current_lane = EAST_LEFT;
                                    my_state->num_out_east_left_++;

                                } else if (traffic_event->getArrivedFrom() == EAST_STRAIGHT) {
                                    current_lane = EAST_STRAIGHT;
                                    my_state->num_out_east_straight_++;

                                } else if (traffic_event->getArrivedFrom() == SOUTH_LEFT) {
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
                                if (traffic_event->getArrivedFrom() == SOUTH_LEFT) {
                                    current_lane = EAST_RIGHT;
                                    my_state->num_out_east_right_++;

                                } else if (traffic_event->getArrivedFrom() == EAST_STRAIGHT) {
                                    current_lane = EAST_STRAIGHT;
                                    my_state->num_out_east_straight_++;

                                } else if (traffic_event->getArrivedFrom() == NORTH_RIGHT) {
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
                                if (traffic_event->getArrivedFrom() == SOUTH_RIGHT) {
                                    current_lane = WEST_LEFT;
                                    my_state->num_out_west_left_++;

                                } else if (traffic_event->getArrivedFrom() == WEST_STRAIGHT) {
                                    current_lane = WEST_STRAIGHT;
                                    my_state->num_out_west_straight_++;

                                } else if (traffic_event->getArrivedFrom() == NORTH_LEFT) {
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
                                if (traffic_event->getArrivedFrom() == SOUTH_RIGHT) {
                                    current_lane = WEST_LEFT;
                                    my_state->num_out_west_left_++;

                                } else if (traffic_event->getArrivedFrom() == WEST_STRAIGHT) {
                                    current_lane = WEST_STRAIGHT;
                                    my_state->num_out_west_straight_++;

                                } else if (traffic_event->getArrivedFrom() == NORTH_LEFT) {
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
                                if (traffic_event->getArrivedFrom() == SOUTH_RIGHT) {
                                    current_lane = WEST_LEFT;
                                    my_state->num_out_west_left_++;

                                } else if (traffic_event->getArrivedFrom() == WEST_STRAIGHT) {
                                    current_lane = WEST_STRAIGHT;
                                    my_state->num_out_west_straight_++;

                                } else if (traffic_event->getArrivedFrom() == NORTH_LEFT) {
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
                                if (traffic_event->getArrivedFrom() == WEST_RIGHT) {
                                    current_lane = NORTH_LEFT;
                                    my_state->num_out_north_left_++;

                                } else if (traffic_event->getArrivedFrom() == NORTH_STRAIGHT) {
                                    current_lane = NORTH_STRAIGHT;
                                    my_state->num_out_north_straight_++;

                                } else if (traffic_event->getArrivedFrom() == EAST_LEFT) {
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
                                if (traffic_event->getArrivedFrom() == WEST_RIGHT) {
                                    current_lane = NORTH_LEFT;
                                    my_state->num_out_north_left_++;

                                } else if (traffic_event->getArrivedFrom() == NORTH_STRAIGHT) {
                                    current_lane = NORTH_STRAIGHT;
                                    my_state->num_out_north_straight_++;

                                } else if (traffic_event->getArrivedFrom() == EAST_LEFT) {
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
                                if (traffic_event->getArrivedFrom() == WEST_RIGHT) {
                                    current_lane = NORTH_LEFT;
                                    my_state->num_out_north_left_++;

                                } else if (traffic_event->getArrivedFrom() == NORTH_STRAIGHT) {
                                    current_lane = NORTH_STRAIGHT;
                                    my_state->num_out_north_straight_++;

                                } else if (traffic_event->getArrivedFrom() == EAST_LEFT) {
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
                                if (traffic_event->getArrivedFrom() == WEST_LEFT) {
                                    current_lane = SOUTH_RIGHT;
                                    my_state->num_out_south_right_++;

                                } else if (traffic_event->getArrivedFrom() == SOUTH_STRAIGHT) {
                                    current_lane = SOUTH_STRAIGHT;
                                    my_state->num_out_south_straight_++;

                                } else if (traffic_event->getArrivedFrom() == EAST_RIGHT) {
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
                                if (traffic_event->getArrivedFrom() == EAST_RIGHT) {
                                    current_lane = SOUTH_LEFT;
                                    my_state->num_out_south_left_++;

                                } else if (traffic_event->getArrivedFrom() == SOUTH_STRAIGHT) {
                                    current_lane = SOUTH_STRAIGHT;
                                    my_state->num_out_south_straight_++;

                                } else if (traffic_event->getArrivedFrom() == WEST_LEFT) {
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
                                if (traffic_event->getArrivedFrom() == EAST_RIGHT) {
                                    current_lane = SOUTH_LEFT;
                                    my_state->num_out_south_left_++;

                                } else if (traffic_event->getArrivedFrom() == SOUTH_STRAIGHT) {
                                    current_lane = SOUTH_STRAIGHT;
                                    my_state->num_out_south_straight_++;

                                } else if (traffic_event->getArrivedFrom() == WEST_LEFT) {
                                    current_lane = SOUTH_RIGHT;
                                    my_state->num_out_south_right_++;
                                }
                            }
                        } break;
                    }

                    IntVTime curr_time = static_cast<const IntVTime&> (getSimulationTime());
                    NegativeExpntl interval_neg_expo(mean_interval_, my_state->gen_);
                    int interval = (int) interval_neg_expo();
                    TrafficEvent *event = 
                        new TrafficEvent(   curr_time, curr_time + interval, this, this,
                                            x_to_go, y_to_go, 
                                            traffic_event->getCurrentLane(), current_lane, 
                                            DEPARTURE   );
                    this->receiveEvent(event);

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

    std::string objectName(unsigned int index) {
        return std::string("Intersection_") + std::to_string(index);
    }

    SimulationObject *computeMove(direction_t direction) {

        unsigned int new_x = 0, new_y = 0;
        unsigned int current_y = this->index_ / num_intersections_x_;
        unsigned int current_x = this->index_ % num_intersections_x_;

        switch (direction) {
            case WEST: {
                new_x = (current_x + num_intersections_x_ - 1) % num_intersections_x_;
                new_y = current_y;
            } break;

            case EAST: {
                new_x = (current_x + 1) % num_intersections_x_;
                new_y = current_y;
            } break;

            case SOUTH: {
                new_x = current_x;
                new_y = (current_y + num_intersections_y_ - 1) % num_intersections_y_;
            } break;

            case NORTH: {
                new_x = current_x;
                new_y = (current_y + 1) % num_intersections_y_;
            } break;

            default: {
                ASSERT(0);
            }
        }
        std::string object_name = objectName(new_x + new_y * num_intersections_x_);

        return getObjectHandle(object_name);
    }
};

#endif
