#ifndef PINGEVENT_HH
#define PINGEVENT_HH

#include <string>

#include <DefaultEvent.h>

class PingEvent : public DefaultEvent {
public:
  /**
     Constructor called by application.
  */
  PingEvent( const VTime &initSendTime,
	     const VTime &initRecvTime,
	     SimulationObject *initSender,
	     SimulationObject *initReceiver,
	     const std::string &owner ) : 
    DefaultEvent( initSendTime, initRecvTime, initSender, initReceiver ),
    myOwner( owner ){}

  
  ~PingEvent(){}

  unsigned int getEventSize() const { return sizeof(PingEvent); } 
  
  static Serializable *deserialize( SerializedInstance *instance );

  void serialize( SerializedInstance * ) const;
  
  bool eventCompare(const Event* event);

  static const std::string &getPingEventDataType(){
    static std::string pingEventDataType = "PingEvent";
    return pingEventDataType;
  }
  
  const std::string &getDataType() const {
    return getPingEventDataType();
  }

  const std::string &getOwner() const { return myOwner; }

private:
  /*
     Constructor called by deserializer.
  */
  PingEvent( const VTime &initSendTime,
	     const VTime &initRecvTime,
	     const ObjectID &initSender,
	     const ObjectID &initReceiver,
	     const unsigned int eventIdVal,
	     const std::string &owner ) : 
    DefaultEvent( initSendTime, initRecvTime, initSender, initReceiver, eventIdVal ),
    myOwner( owner ){}


  const std::string myOwner;
};

#endif
