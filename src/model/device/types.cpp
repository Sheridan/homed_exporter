#include "model/device/types.h"
#include "types.h"

namespace he
{
namespace model
{
namespace device
{

std::string to_string(const EDeviceRole &role)
{
  switch (role)
  {
    case EDeviceRole::drCoordinator: return "coordinator";
    case EDeviceRole::drEndpoint   : return "endpoint";
    case EDeviceRole::drRouter     : return "router";
    case EDeviceRole::drUnknown    : return "unknown";
  }
  return "unknown";
}

std::string to_string(const EDevicePowerSource &powerSource)
{
  switch (powerSource)
  {
    case EDevicePowerSource::psBattery: return "battery";
    case EDevicePowerSource::psDc     : return "dc";
    case EDevicePowerSource::psMains  : return "mains";
    case EDevicePowerSource::psUnknown: return "unknown";
  }
  return "unknown";
}

EDeviceRole to_role(const Json::Value &data)
{
  if(data.isNull()) { return EDeviceRole::drUnknown; }
  return static_cast<EDeviceRole>(data.asUInt());
}

EDevicePowerSource to_power_source(const Json::Value &data)
{
  if(data.isNull()) { return EDevicePowerSource::psUnknown; }
  return static_cast<EDevicePowerSource>(data.asUInt());
}
}
}
}
