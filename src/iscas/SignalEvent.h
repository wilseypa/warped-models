#ifndef SIGNAL_EVENT_H
#define SIGNAL_EVENT_H

#include <DefaultEvent.h>
#include <string>

class VTime;
class SimulationObject;
class ObjectID;
class Serializable;
class SerializedInstance;

class SignalEvent : public DefaultEvent {
public:
    SignalEvent(const VTime& initSendTime,
                const VTime& initRecvTime,
                SimulationObject* initSender,
                SimulationObject* initReceiver,
                unsigned int inputIndex,
                bool inputValue);

    static const std::string& getSignalEventDataType() {
        static std::string eventDataType = "SignalEvent";
        return eventDataType;
    }

    const std::string& getDataType() const {
        return getSignalEventDataType();
    }

    unsigned int getInputIndex() const { return inputIndex; }
    bool getInputValue() const { return inputValue; }
    unsigned int getEventSize() const { return sizeof(SignalEvent); }

    static Serializable* deserialize(SerializedInstance* instance);
    void serialize(SerializedInstance* instance) const;
    bool eventCompare(const Event* event);

private:
    /// Constructor called by deserializer.
    SignalEvent(const VTime& initSendTime,
              const VTime& initRecvTime,
              const ObjectID& initSender,
              const ObjectID& initReceiver,
              unsigned int eventIdVal,
              unsigned int inputIndex,
              bool inputValue);

    unsigned int inputIndex;
    bool inputValue;

};
#endif