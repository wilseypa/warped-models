#include "SignalEvent.h"

#include "Event.h"
#include "ObjectID.h"
#include "SerializedInstance.h"
#include "SimulationObject.h"
#include "VTime.h"

SignalEvent::SignalEvent(const VTime& initSendTime,
                         const VTime& initRecvTime,
                         SimulationObject* initSender,
                         SimulationObject* initReceiver,
                         unsigned int inputIndex,
                         bool inputValue)
    : DefaultEvent(initSendTime, initRecvTime, initSender, initReceiver),
      inputIndex(inputIndex), inputValue(inputValue) {}

/// Constructor called by deserializer.
SignalEvent::SignalEvent(const VTime& initSendTime,
                         const VTime& initRecvTime,
                         const ObjectID& initSender,
                         const ObjectID& initReceiver,
                         unsigned int eventIdVal,
                         unsigned int inputIndex,
                         bool inputValue)
    : DefaultEvent(initSendTime, initRecvTime, initSender, initReceiver, eventIdVal),
      inputIndex(inputIndex), inputValue(inputValue) {}

Serializable* SignalEvent::deserialize(SerializedInstance* instance) {
    VTime* sendTime = dynamic_cast<VTime*>(instance->getSerializable());
    VTime* receiveTime = dynamic_cast<VTime*>(instance->getSerializable());
    unsigned int senderSimManID = instance->getUnsigned();
    unsigned int senderSimObjID = instance->getUnsigned();
    unsigned int receiverSimManID = instance->getUnsigned();
    unsigned int receiverSimObjID = instance->getUnsigned();
    unsigned int eventId = instance->getUnsigned();
    unsigned int inputIndex = instance->getUnsigned();
    bool inputValue = instance->getUnsigned() != 0;

    ObjectID sender(senderSimObjID, senderSimManID);
    ObjectID receiver(receiverSimObjID, receiverSimManID);

    SignalEvent* event = new SignalEvent(*sendTime, *receiveTime, sender, receiver,
                                         eventId, inputIndex, inputValue);
    delete sendTime;
    delete receiveTime;

    return event;
}

void SignalEvent::serialize(SerializedInstance* instance) const {
    Event::serialize(instance);
    instance->addUnsigned(inputIndex);
    instance->addUnsigned(inputValue);
}

bool SignalEvent::eventCompare(const Event* event) {
    if (compareEvents(this, event)) {
        auto otherEvent = dynamic_cast<const SignalEvent*>(event);
        if (otherEvent != nullptr) {
            return otherEvent->getInputIndex() == inputIndex &&
                   otherEvent->getInputValue() == inputValue;
        }
    }
    return false;
}

