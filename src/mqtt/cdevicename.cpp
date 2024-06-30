#include "mqtt/cdevicename.h"
#include "st.h"

#include <string_view>

namespace he
{
namespace mqtt
{

CDeviceName::CDeviceName(const std::string &topic, const std::string &path)
  : m_name(""), m_endpoint("")
{
  extractNameParts(eraseRoot(topic, HE_ST.config().mqttHomedTopic() + path));
}

std::string CDeviceName::eraseRoot(std::string topic, const std::string &root)
{
  size_t pos;
  while ((pos = topic.find(root)) != std::string::npos)
  {
      topic.erase(pos, root.length());
  }
  return topic;
}

void CDeviceName::extractNameParts(const std::string &unrooted)
{
    std::size_t pos = unrooted.find("/");
    if (pos != std::string_view::npos)
    {
        m_name = unrooted.substr(0, pos);
        m_endpoint = unrooted.substr(pos + 1);
    }
    else
    {
        m_name = unrooted;
    }
}

}
}
