#ifndef TRAFFIC_OBJECT_STATE_H
#define TRAFFIC_OBJECT_STATE_H

#include <State.h>
#include <sstream>
#include <string>

class TrafficObjectState : public State {

public :
    TrafficObjectState() :
        total_cars_arrived_(0),
        total_cars_finished_(0), 
        num_in_north_left_(0),
        num_in_north_straight_(0),
        num_in_north_right_(0),
        num_in_south_left_(0),
        num_in_south_straight_(0),
        num_in_south_right_(0),
        num_in_east_left_(0),
        num_in_east_straight_(0),
        num_in_east_right_(0),
        num_in_west_left_(0),
        num_in_west_straight_(0),
        num_in_west_right_(0),
        num_out_north_left_(0),
        num_out_north_straight_(0),
        num_out_north_right_(0),
        num_out_south_left_(0),
        num_out_south_straight_(0),
        num_out_south_right_(0),
        num_out_east_left_(0),
        num_out_east_straight_(0),
        num_out_east_right_(0),
        num_out_west_left_(0),
        num_out_west_straight_(0),
        num_out_west_right_(0) {}

    ~TrafficObjectState() {};

    void copyState( const State *to_copy ) {
        ASSERT(to_copy);
        const TrafficObjectState *traffic_state = dynamic_cast<const TrafficObjectState *>(to_copy);
        total_cars_arrived_ = traffic_state->total_cars_arrived_;
        total_cars_finished_ = traffic_state->total_cars_finished_;
        num_in_north_left_ = traffic_state->num_in_north_left_;
        num_in_north_straight_ = traffic_state->num_in_north_straight_;
        num_in_north_right_ = traffic_state->num_in_north_right_;
        num_in_south_left_ = traffic_state->num_in_south_left_;
        num_in_south_straight_ = traffic_state->num_in_south_straight_;
        num_in_south_right_ = traffic_state->num_in_south_right_;
        num_in_east_left_ = traffic_state->num_in_east_left_;
        num_in_east_straight_ = traffic_state->num_in_east_straight_;
        num_in_east_right_ = traffic_state->num_in_east_right_;
        num_in_west_left_ = traffic_state->num_in_west_left_;
        num_in_west_straight_ = traffic_state->num_in_west_straight_;
        num_in_west_right_ = traffic_state->num_in_west_right_;
        num_out_north_left_ = traffic_state->num_out_north_left_;
        num_out_north_straight_ = traffic_state->num_out_north_straight_;
        num_out_north_right_ = traffic_state->num_out_north_right_;
        num_out_south_left_ = traffic_state->num_out_south_left_;
        num_out_south_straight_ = traffic_state->num_out_south_straight_;
        num_out_south_right_ = traffic_state->num_out_south_right_;
        num_out_east_left_ = traffic_state->num_out_east_left_;
        num_out_east_straight_ = traffic_state->num_out_east_straight_;
        num_out_east_right_ = traffic_state->num_out_east_right_;
        num_out_west_left_ = traffic_state->num_out_west_left_;
        num_out_west_straight_ = traffic_state->num_out_west_straight_;
        num_out_west_right_ = traffic_state->num_out_west_right_;
    }

    unsigned int total_cars_arrived_;
    unsigned int total_cars_finished_;
    unsigned int num_in_north_left_;
    unsigned int num_in_north_straight_;
    unsigned int num_in_north_right_;
    unsigned int num_in_south_left_;
    unsigned int num_in_south_straight_;
    unsigned int num_in_south_right_;
    unsigned int num_in_east_left_;
    unsigned int num_in_east_straight_;
    unsigned int num_in_east_right_;
    unsigned int num_in_west_left_;
    unsigned int num_in_west_straight_;
    unsigned int num_in_west_right_;
    unsigned int num_out_north_left_;
    unsigned int num_out_north_straight_;
    unsigned int num_out_north_right_;
    unsigned int num_out_south_left_;
    unsigned int num_out_south_straight_;
    unsigned int num_out_south_right_;
    unsigned int num_out_east_left_;
    unsigned int num_out_east_straight_;
    unsigned int num_out_east_right_;
    unsigned int num_out_west_left_;
    unsigned int num_out_west_straight_;
    unsigned int num_out_west_right_;
};

#endif
