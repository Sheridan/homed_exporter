#pragma once
#include "model/device/cdevice.h"
#include "mqtt/ctopic.h"
#include <jsoncpp/json/json.h>
#include <list>
#include <string>

namespace he
{
namespace model
{
namespace device
{

typedef std::list<CDevice *> TDevices;

class CDevices
{
public:
  CDevices();
  ~CDevices();

  void update(const he::mqtt::CTopic *topic, const Json::Value &data);
  bool exists(const std::string& name);
  CDevice *get(const std::string& name);
  CDevice *get(const TNetworkAddress& address);
  bool empty();

private:
  TDevices m_devices;
  bool mustIgnore(const std::string &deviceName, const std::string &endpointName = "");

};



}
}
}
