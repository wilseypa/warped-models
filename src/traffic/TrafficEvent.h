#ifndef EPIDEMIC_EVENT_H
#define EPIDEMIC_EVENT_H

#include "DefaultEvent.h"
#include "IntVTime.h"
#include "SerializedInstance.h"


enum event_type_t {

    ARRIVAL,
    DEPARTURE,
    DIRECTION_SELECT
};

enum car_direction_t {

    NORTH_LEFT, 
    NORTH_STRAIGHT, 
    NORTH_RIGHT, 
    SOUTH_LEFT, 
    SOUTH_STRAIGHT, 
    SOUTH_RIGHT, 
    EAST_LEFT, 
    EAST_STRAIGHT, 
    EAST_RIGHT, 
    WEST_LEFT, 
    WEST_STRAIGHT, 
    WEST_RIGHT
};

class TrafficEvent: public DefaultEvent {

public:

    /* Constructor */
    TrafficEvent(   const VTime &send_time,
                    const VTime &recv_time,
                    SimulationObject *sender,
                    SimulationObject *receiver,
                    const int x_to_go,
                    const int y_to_go,
                    const car_direction_t arrived_from,
                    const car_direction_t current_lane,
                    const event_type_t event_type ) : 
            DefaultEvent( send_time, recv_time, sender, receiver ),
            x_to_go_(x_to_go),
            y_to_go_(y_to_go),
            arrived_from_(arrived_from),
            current_lane_(current_lane),
            event_type_(event_type) {}

    /* Destructor */
    ~TrafficEvent() {}

    static Serializable* deserialize( SerializedInstance *instance ) {

        VTime *send_time    = dynamic_cast<VTime *>(instance->getSerializable());
        VTime *receive_time = dynamic_cast<VTime *>(instance->getSerializable());
        unsigned int sender_manager_id   = instance->getUnsigned();
        unsigned int sender_object_id    = instance->getUnsigned();
        unsigned int receiver_manager_id = instance->getUnsigned();
        unsigned int receiver_object_id  = instance->getUnsigned();
        unsigned int event_id            = instance->getUnsigned();

        ObjectID sender(sender_object_id, sender_manager_id);
        ObjectID receiver(receiver_object_id, receiver_manager_id);

        TrafficEvent *event = new TrafficEvent(
                *send_time, *receive_time, sender, receiver, event_id);   

        event->setX ( instance->getUnsigned() );
        event->setY ( instance->getUnsigned() );
        event->setArrivedFrom ( instance->getInt() );
        event->setCurrentLane ( instance->getInt() );
        event->setEventType ( instance->getInt() );

        delete send_time;
        delete receive_time;

        return event;
    }

    void serialize( SerializedInstance *add_to ) const {
        Event::serialize(add_to);
        add_to->addUnsigned(x_to_go_);
        add_to->addUnsigned(y_to_go_);
        add_to->addInt(arrived_from_);
        add_to->addInt(current_lane_);
        add_to->addInt(event_type_);
    }

    bool eventCompare( const Event* event ) {

        TrafficEvent *this_event = (TrafficEvent *) event;

        return (    compareEvents ( this, event ) && 
                    ( x_to_go_ == this_event->getX() ) &&
                    ( y_to_go_ == this_event->getY() ) &&
                    ( arrived_from_ == this_event->getArrivedFrom() ) &&
                    ( current_lane_ == this_event->getCurrentLane() ) &&
                    ( event_type_ == this_event->getEventType() )
               );
    }

    static const string &getTrafficEventDataType(){
        static std::string traffic_event_data_type = "TrafficEvent";
        return traffic_event_data_type;
    }

    const string &getDataType() const {
        return getTrafficEventDataType();
    }

    unsigned int getEventSize() const {
        return sizeof(TrafficEvent);
    }

    void setX( unsigned int x_to_go ) {
        x_to_go_ = x_to_go;
    }

    unsigned int getX() {
        return x_to_go_;
    }

    void setY( unsigned int y_to_go ) {
        y_to_go_ = y_to_go;
    }

    unsigned int getY() {
        return y_to_go_;
    }

    void setArrivedFrom(int arrived_from) { 
        arrived_from_ = (car_direction_t) arrived_from;
    }

    car_direction_t getArrivedFrom() {
        return arrived_from_;
    }

    void setCurrentLane(int current_lane) { 
        current_lane_ = (car_direction_t) current_lane;
    }

    car_direction_t getCurrentLane() {
        return current_lane_;
    }

    void setEventType( int event_type ) {
        event_type_ = (event_type_t) event_type;
    }

    event_type_t getEventType() {
        return event_type_;
    }

private:

    TrafficEvent(   const VTime &initSendTime,
                    const VTime &initRecvTime,
                    const ObjectID &initSender,
                    const ObjectID &initReceiver,
                    const unsigned int eventId  ) :
            DefaultEvent( initSendTime, initRecvTime, initSender, initReceiver, eventId ),
            x_to_go_(0), y_to_go_(0), 
            arrived_from_((car_direction_t)0), 
            current_lane_((car_direction_t)0), 
            event_type_((event_type_t)0) {}

    int x_to_go_;
    int y_to_go_;
    car_direction_t arrived_from_;
    car_direction_t current_lane_;
    event_type_t event_type_;
};

#endif
