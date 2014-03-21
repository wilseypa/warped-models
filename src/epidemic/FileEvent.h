#ifndef FILE_EVENT_H
#define FILE_EVENT_H

#include "DefaultEvent.h"
#include "IntVTime.h"
#include "SerializedInstance.h"

class FileEvent: public DefaultEvent {

public:

    /* Constructor */
    FileEvent(  const VTime &initSendTime,
                const VTime &initRecvTime,
                SimulationObject *initSender,
                SimulationObject *initReceiver) : 
            DefaultEvent( initSendTime, initRecvTime, initSender, initReceiver ) {}

    /* Destructor */
    ~FileEvent() {}

    static Serializable* deserialize( SerializedInstance *instance ) {

        VTime *sendTime = dynamic_cast<VTime *>(instance->getSerializable());
        VTime *receiveTime = dynamic_cast<VTime *>(instance->getSerializable());
        unsigned int senderSimManID = instance->getUnsigned();
        unsigned int senderSimObjID = instance->getUnsigned();
        unsigned int receiverSimManID = instance->getUnsigned();
        unsigned int receiverSimObjID = instance->getUnsigned();
        unsigned int eventId          = instance->getUnsigned();

        ObjectID sender(senderSimObjID, senderSimManID);
        ObjectID receiver(receiverSimObjID, receiverSimManID);

        FileEvent *event = new FileEvent(*sendTime, *receiveTime, sender, receiver, eventId);   

        delete sendTime;
        delete receiveTime;

        return event;
    }

    void serialize( SerializedInstance *addTo ) const {
        Event::serialize(addTo);
    }

    bool eventCompare( const Event* event ) {

        FileEvent *thisEvent = (FileEvent *) event;

        return ( compareEvents ( this, event ) ); 
    }

    static const string &getFileEventDataType(){
        static string fileEventDataType = "FileEvent";
        return fileEventDataType;
    }

    const string &getDataType() const { return getFileEventDataType(); }

    unsigned int getEventSize() const { return sizeof(FileEvent); }

private:

    FileEvent(  const VTime &initSendTime,
                const VTime &initRecvTime,
                const ObjectID &initSender,
                const ObjectID &initReceiver,
                const unsigned int eventId  ) :
            DefaultEvent( initSendTime, initRecvTime, initSender, initReceiver, eventId ) {}
};

#endif
