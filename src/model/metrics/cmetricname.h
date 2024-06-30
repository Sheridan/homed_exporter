#pragma once
#include <string>
#include <string_view>
#include "mqtt/ctopic.h"

namespace he
{
namespace model
{
namespace metrics
{

class CMetricName
{
public:
  static std::string metricName(const std::string &name, const he::mqtt::ETopic &topicType = he::mqtt::ETopic::tUnknown);
private:
  static constexpr std::string_view staticPart = "homed_";
  static std::string metricTopicNamePart(const he::mqtt::ETopic &topicType);
};


}
}
}
