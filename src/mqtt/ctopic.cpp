#include "mqtt/ctopic.h"
#include "st.h"

#include <sstream>
#include <iostream>
#include "ctopic.h"
// #include <tuple>

namespace he
{
namespace mqtt
{

CTopic::CTopic(const std::string &topic)
  : m_root(HE_ST.config().mqttHomedTopic()),
    m_topic(""),
    m_service(""),
    m_device(""),
    m_deviceEndpoint("")
{
  parseTopic(topic);
  m_topicType = mineTopicType();
  m_serviceType = mineServiceType();

  // HE_LOG_DBG(m_root << "/" << m_topic << "/" << m_service << "/" << m_device << "/" << m_deviceEndpoint << " : " << m_topicType);
}
CTopic::~CTopic() {}

void CTopic::parseTopic(const std::string &topic)
{
  std::string root;
  std::istringstream stream(topic);
  std::getline(stream, root            , '/');
  std::getline(stream, m_topic         , '/');
  std::getline(stream, m_service       , '/');
  std::getline(stream, m_device        , '/');
  std::getline(stream, m_deviceEndpoint, '/');
}

ETopic CTopic::mineTopicType()
{
  if(m_topic == "command") { return ETopic::tCommand; }
  if(m_topic == "device" ) { return ETopic::tDevice;  }
  if(m_topic == "event"  ) { return ETopic::tEvent;   }
  if(m_topic == "expose" ) { return ETopic::tExpose;  }
  if(m_topic == "fd"     ) { return ETopic::tFd;      }
  if(m_topic == "service") { return ETopic::tService; }
  if(m_topic == "status" ) { return ETopic::tStatus;  }
  if(m_topic == "td"     ) { return ETopic::tTd;      }
  return ETopic::tUnknown;
}

EService CTopic::mineServiceType()
{
  if(m_service == "zigbee") { return EService::sZigbee; }
  if(m_service == "custom") { return EService::sCustom; }
  if(m_service == "web"   ) { return EService::sWeb;    }
  if(m_service == "cloud" ) { return EService::sCloud;  }
  return EService::sUnknown;
}

}
}
