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
                SimulationObject *initReceiver,
                string locationName,
                unsigned int dataCaptureIndex,
                unsigned int uninfectedNum,
                unsigned int latentNum,
                unsigned int incubatingNum,
                unsigned int infectiousNum,
                unsigned int asymptNum,
                unsigned int recoveredNum ) : 
            DefaultEvent( initSendTime, initRecvTime, initSender, initReceiver ),
            locationName(locationName),
            dataCaptureIndex(dataCaptureIndex),
            uninfectedNum(uninfectedNum),
            latentNum(latentNum),
            incubatingNum(incubatingNum),
            infectiousNum(infectiousNum),
            asymptNum(asymptNum),
            recoveredNum(recoveredNum) {
    }

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

        event->setLocationName ( instance->getString() );
        event->setDataCaptureIndex ( instance->getUnsigned() );
        event->setUninfectedNum ( instance->getUnsigned() );
        event->setLatentNum ( instance->getUnsigned() );
        event->setIncubatingNum ( instance->getUnsigned() );
        event->setInfectiousNum ( instance->getUnsigned() );
        event->setAsymptNum ( instance->getUnsigned() );
        event->setRecoveredNum ( instance->getUnsigned() );

        delete sendTime;
        delete receiveTime;

        return event;
    }

    void serialize( SerializedInstance *addTo ) const {
        Event::serialize(addTo);
        addTo->addString(locationName);
        addTo->addUnsigned(dataCaptureIndex);
        addTo->addUnsigned(uninfectedNum);
        addTo->addUnsigned(latentNum);
        addTo->addUnsigned(incubatingNum);
        addTo->addUnsigned(infectiousNum);
        addTo->addUnsigned(asymptNum);
        addTo->addUnsigned(recoveredNum);
    }

    bool eventCompare( const Event* event ) {

        FileEvent *thisEvent = (FileEvent *) event;

        return ( compareEvents ( this, event ) && 
                 (locationName == thisEvent->getLocationName()) && 
                 (dataCaptureIndex == thisEvent->getDataCaptureIndex()) && 
                 (uninfectedNum == thisEvent->getUninfectedNum()) &&
                 (latentNum == thisEvent->getLatentNum()) &&
                 (incubatingNum == thisEvent->getIncubatingNum()) &&
                 (infectiousNum == thisEvent->getInfectiousNum()) &&
                 (asymptNum == thisEvent->getAsymptNum()) &&
                 (recoveredNum == thisEvent->getRecoveredNum()) ); 
    }

    static const string &getFileEventDataType(){
        static string fileEventDataType = "FileEvent";
        return fileEventDataType;
    }

    const string &getDataType() const { return getFileEventDataType(); }

    unsigned int getEventSize() const { return sizeof(FileEvent); }

    void setLocationName( const string locName ) { locationName = locName; }

    const string &getLocationName() { return locationName; }

    void setDataCaptureIndex( unsigned int dataIndex ) { dataCaptureIndex = dataIndex; }

    unsigned int getDataCaptureIndex() { return dataCaptureIndex; }

    void setUninfectedNum( unsigned int uninfectedCnt ) { uninfectedNum = uninfectedCnt; }

    unsigned int getUninfectedNum() { return uninfectedNum; }

    void setLatentNum( unsigned int latentCnt ) { latentNum = latentCnt; }

    unsigned int getLatentNum() { return latentNum; }

    void setIncubatingNum( unsigned int incubatingCnt ) { incubatingNum = incubatingCnt; }

    unsigned int getIncubatingNum() { return incubatingNum; }

    void setInfectiousNum( unsigned int infectiousCnt ) { infectiousNum = infectiousCnt; }

    unsigned int getInfectiousNum() { return infectiousNum; }

    void setAsymptNum( unsigned int asymptCnt ) { asymptNum = asymptCnt; }

    unsigned int getAsymptNum() { return asymptNum; }

    void setRecoveredNum( unsigned int recoveredCnt ) { recoveredNum = recoveredCnt; }

    unsigned int getRecoveredNum() { return recoveredNum; }

private:

    FileEvent(  const VTime &initSendTime,
                const VTime &initRecvTime,
                const ObjectID &initSender,
                const ObjectID &initReceiver,
                const unsigned int eventId  ) :
            DefaultEvent( initSendTime, initRecvTime, initSender, initReceiver, eventId ), 
            locationName(""), dataCaptureIndex(0), uninfectedNum(0), latentNum(0),
            incubatingNum(0), infectiousNum(0), asymptNum(0), recoveredNum(0) {
    }

    /* Location name */
    string locationName;

    /* Data capture index */
    unsigned int dataCaptureIndex;

    /* Uninfected population count */
    unsigned int uninfectedNum;

    /* Latent population count */
    unsigned int latentNum;

    /* Incubating population count */
    unsigned int incubatingNum;

    /* Infectious population count */
    unsigned int infectiousNum;

    /* Asympt population count */
    unsigned int asymptNum;

    /* Recovered population count */
    unsigned int recoveredNum;
};

#endif
