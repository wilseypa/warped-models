#ifndef CLOCK_EVENT_H
#define CLOCK_EVENT_H

#include <DefaultEvent.h>
#include <string>

class Serializable;
class SerializedInstance;

class ClockEvent : public DefaultEvent {
public:
    ClockEvent(const VTime& initSendTime, const VTime& initRecvTime,
               SimulationObject* initSender, SimulationObject* initReceiver);

    static const std::string& getClockEventDataType() {
        static std::string eventDataType = "ClockEvent";
        return eventDataType;
    }

    const std::string& getDataType() const {
        return getClockEventDataType();
    }

    unsigned int getEventSize() const { return sizeof(ClockEvent); }

    static Serializable* deserialize(SerializedInstance* instance);
    bool eventCompare(const Event* event);

private:
    // Constructor called by deserializer.
    ClockEvent(const VTime& initSendTime,
                const VTime& initRecvTime,
                const ObjectID& initSender,
                const ObjectID& initReceiver,
                unsigned int eventIdVal);
};
#endif