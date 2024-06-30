#pragma once
#include <string>
#include <jsoncpp/json/json.h>

namespace he
{
namespace model
{
namespace device
{

typedef unsigned short TNetworkAddress;

enum EDeviceRole
{
  drCoordinator = 0,
  drRouter      = 1,
  drEndpoint    = 2,
  drUnknown     = 64
};

enum EDevicePowerSource
{
  psUnknown = 0,
  psMains   = 1,
  psBattery = 3,
  psDc      = 4
};

std::string to_string(const EDeviceRole &role);
std::string to_string(const EDevicePowerSource &powerSource);

EDeviceRole to_role(const Json::Value &data);
EDevicePowerSource to_power_source(const Json::Value &data);

}
}
}
