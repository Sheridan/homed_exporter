#pragma once
#include <string>
#include "defines/class_helpers.h"

namespace he
{
namespace mqtt
{

enum ETopic
{
  tCommand,
  tDevice,
  tEvent,
  tExpose,
  tFd,
  tService,
  tStatus,
  tTd,
  tUnknown
};

enum EService
{
  sZigbee,
  sCustom,
  sWeb,
  sCloud,
  sUnknown
};

class CTopic
{
public:
  CTopic(const std::string &topic);
  ~CTopic();

  HE_SIMPLE_GETTER(ETopic     , topicType     );
  HE_SIMPLE_GETTER(EService   , serviceType   );
  HE_SIMPLE_GETTER(std::string, root          );
  HE_SIMPLE_GETTER(std::string, topic         );
  HE_SIMPLE_GETTER(std::string, service       );
  HE_SIMPLE_GETTER(std::string, device        );
  HE_SIMPLE_GETTER(std::string, deviceEndpoint);

private:
  std::string m_root;
  std::string m_topic;
  std::string m_service;
  std::string m_device;
  std::string m_deviceEndpoint;
  ETopic m_topicType;
  EService m_serviceType;

  void parseTopic(const std::string &topic);
  ETopic mineTopicType();
  EService mineServiceType();
};


}
}
