#include "model/device/cdevices.h"
#include "cdevices.h"
#include "st.h"

namespace he
{
namespace model
{
namespace device
{
CDevices::CDevices()
{}

CDevices::~CDevices()
{
  for(CDevice *device : m_devices)
  {
    delete device;
  }
  m_devices.clear();
}

void CDevices::update(const he::mqtt::CTopic *topic, const Json::Value &data)
{
  switch(topic->topicType())
  {
    case he::mqtt::ETopic::tStatus:
    {
      for(const Json::Value &device : data)
      {
        std::string name = device["name"].asString();
        HE_LOG_DBG("First device update: " << name);
        if(mustIgnore(name)) { continue; }
        if(!exists(name))
        {
          m_devices.push_back(new CDevice(name));
        }
        get(name)->update(topic, device);
      }
    } break;
    case he::mqtt::ETopic::tExpose:
    case he::mqtt::ETopic::tFd:
    case he::mqtt::ETopic::tDevice:
    {
      if(topic->device().empty() || mustIgnore(topic->device(), topic->deviceEndpoint()) || !exists(topic->device())) { return; }
      // HE_LOG_DBG("Can update")
      get(topic->device())->update(topic, data);
    } break;
  }

}

bool CDevices::exists(const std::string &name)
{
  return get(name) != nullptr;
}

CDevice *CDevices::get(const std::string &name)
{
  for(CDevice *device : m_devices)
  {
    if(device->name() == name) { return device; }
  }
  return nullptr;
}

CDevice *CDevices::get(const TNetworkAddress &address)
{
  for(CDevice *device : m_devices)
  {
    if(device->networkAddress() == address) { return device; }
  }
  return nullptr;
}

bool CDevices::empty()
{
  return m_devices.empty();
}

bool CDevices::mustIgnore(const std::string &deviceName, const std::string &endpointName)
{
  // devices
  if(endpointName.empty())
  {
    if(!HE_ST.config().ignoresExists("devices")) { return false; }
    for(const Json::Value &jDevice : HE_ST.config().ignores("devices"))
    {
      if(jDevice.asString() == deviceName) { return true; }
    }
    return false;
  }

  // endpoints
  if(!HE_ST.config().ignoresExists("enpoints")) { return false; }
  const Json::Value &jDevices = HE_ST.config().ignores("enpoints");
  if(jDevices.isMember(deviceName))
  {
    for(const Json::Value &jEndpoint : jDevices[deviceName])
    {
      if(jEndpoint.asString() == endpointName) { return true; }
    }
  }
  return false;
}

}
}
}
