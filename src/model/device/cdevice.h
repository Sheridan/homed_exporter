#pragma once
#include <string>
#include <map>
#include <vector>
#include <jsoncpp/json/json.h>

#include "model/metrics/cmetricbulk.h"
#include "defines/class_helpers.h"
#include "model/device/types.h"
#include "mqtt/ctopic.h"



namespace he
{
namespace model
{
namespace device
{

class CDevice
{
public:
  CDevice(const std::string &name);
  ~CDevice();

  void update(const he::mqtt::CTopic *topic, const Json::Value &data);

  HE_SIMPLE_GETTER(std::string       , name            );
  HE_SIMPLE_GETTER(std::string       , modelName       );
  HE_SIMPLE_GETTER(std::string       , manufacturerName);
  HE_SIMPLE_GETTER(std::string       , description     );
  HE_SIMPLE_GETTER(EDevicePowerSource, powerSource     );
  HE_SIMPLE_GETTER(TNetworkAddress   , networkAddress  );
  HE_SIMPLE_GETTER(EDeviceRole       , role            );

private:
  std::string        m_name;
  std::string        m_modelName;
  std::string        m_manufacturerName;
  std::string        m_description;
  EDevicePowerSource m_powerSource;
  TNetworkAddress    m_networkAddress;
  EDeviceRole        m_role;

  he::model::metrics::TMetricLabels defaultLabels();
  std::string metricHelpSensor(const std::string &itemName, const Json::Value &data);
  std::string metricHelpNumber(const std::string &itemName, const Json::Value &data);

  void updateStatus(const he::mqtt::CTopic *topic, const Json::Value &data);
  void updateExpose(const he::mqtt::CTopic *topic, const Json::Value &data);
  void updateFd    (const he::mqtt::CTopic *topic, const Json::Value &data);
  void updateMain  (const he::mqtt::CTopic *topic, const Json::Value &data, const std::vector<std::string> &keys);
};

}
}
}
