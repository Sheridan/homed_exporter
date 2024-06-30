#pragma once
#include <string>

namespace he
{
namespace mqtt
{

class CDeviceName
{
public:
  CDeviceName(const std::string &topic, const std::string &path);
  ~CDeviceName() {};
  const std::string &name() const { return m_name; }
  const std::string &endpoint() const { return m_endpoint; }

private:
  std::string m_name;
  std::string m_endpoint;

  std::string eraseRoot(std::string topic, const std::string &root);
  void extractNameParts(const std::string &unrooted);
};




}
}
