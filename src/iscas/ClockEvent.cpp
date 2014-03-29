#include "ClockEvent.h"
#include "Event.h"
#include "ObjectID.h"
#include "SerializedInstance.h"
#include "SimulationObject.h"
#include "VTime.h"
#include <string>

class VTime;
class SimulationObject;
class ObjectID;
class Serializable;
class SerializedInstance;

ClockEvent::ClockEvent(const VTime& initSendTime,
                       const VTime& initRecvTime,
                       SimulationObject* initSender,
                       SimulationObject* initReceiver)
    : DefaultEvent(initSendTime, initRecvTime, initSender, initReceiver) {}

/// Constructor called by deserializer.
ClockEvent::ClockEvent(const VTime& initSendTime,
                         const VTime& initRecvTime,
                         const ObjectID& initSender,
                         const ObjectID& initReceiver,
                         unsigned int eventIdVal)
    : DefaultEvent(initSendTime, initRecvTime, initSender, initReceiver, eventIdVal) {}

Serializable* ClockEvent::deserialize(SerializedInstance* instance) {
    VTime* sendTime = dynamic_cast<VTime*>(instance->getSerializable());
    VTime* receiveTime = dynamic_cast<VTime*>(instance->getSerializable());
    unsigned int senderSimManID = instance->getUnsigned();
    unsigned int senderSimObjID = instance->getUnsigned();
    unsigned int receiverSimManID = instance->getUnsigned();
    unsigned int receiverSimObjID = instance->getUnsigned();
    unsigned int eventId = instance->getUnsigned();

    ObjectID sender(senderSimObjID, senderSimManID);
    ObjectID receiver(receiverSimObjID, receiverSimManID);

    ClockEvent* event = new ClockEvent(*sendTime, *receiveTime, sender,
                                       receiver, eventId);
    delete sendTime;
    delete receiveTime;

    return event;
}

bool ClockEvent::eventCompare(const Event* event) {
    return compareEvents(this, event);
}
