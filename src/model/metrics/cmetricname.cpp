#include "model/metrics/cmetricname.h"
#include "utils/string.h"
#include "cmetricname.h"

namespace he
{
namespace model
{
namespace metrics
{

std::string CMetricName::metricName(const std::string &name, const he::mqtt::ETopic &topicType)
{
  return std::string(staticPart) + metricTopicNamePart(topicType) + he::utils::to_snake_case(name);
}

std::string CMetricName::metricTopicNamePart(const he::mqtt::ETopic &topicType)
{
  switch(topicType)
  {
    case he::mqtt::ETopic::tDevice: return "device_"; break;
    case he::mqtt::ETopic::tFd: return "zigbee_"; break;
    case he::mqtt::ETopic::tStatus: return "status_"; break;
  }
  return "";
}

}
}
}
