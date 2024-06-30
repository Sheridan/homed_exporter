#include "mqtt/cmqttcallback.h"
#include "mqtt/cpayload.h"
#include "mqtt/ctopic.h"
#include "mqtt/cdevicename.h"
#include "st.h"
#include "cmqttcallback.h"

namespace he
{
namespace mqtt
{

CMqttCallback::CMqttCallback()
{}
CMqttCallback::~CMqttCallback()
{}

void CMqttCallback::message_arrived(::mqtt::const_message_ptr msg)
{
  // HE_LOG_NFO << "Message arrived:\n"
  //           << "\ttopic: '" << msg->get_topic() << "'\n"
  //           << "\tpayload: '" << msg->get_payload_str() << "'\n" << std::endl;
  CPayload *payload = new CPayload();
  if(payload->parse(msg->get_payload_str()))
  {
    CTopic *topic = new CTopic(msg->get_topic());
    if(topic->topicType() != ETopic::tUnknown && topic->serviceType() == EService::sZigbee)
    {
      HE_LOG_VERBOSE("Incoming data from topic " << topic->topic());
      HE_ST.homed().update(topic, payload->value());
    }
    delete topic;
  }
  delete payload;
}

}
}
