#include "model/device/cdevice.h"
#include "cdevice.h"
#include "st.h"
#include "model/metrics/cmetricname.h"
#include "defines/metrics.h"

// #include <format>
namespace he
{
namespace model
{
namespace device
{

CDevice::CDevice(const std::string &name)
  : m_name(name),
    m_modelName(""),
    m_manufacturerName(""),
    m_description(""),
    m_powerSource(psUnknown),
    m_networkAddress(0),
    m_role(drCoordinator)
{}

CDevice::~CDevice() {}

#define HE_CASE_PROCESS(_type) \
    case he::mqtt::ETopic::t##_type: \
    { \
      /* HE_LOG_DBG("Processing " #_type); */ \
      update##_type(topic, data); \
    }; return;


void CDevice::update(const he::mqtt::CTopic *topic, const Json::Value &data)
{
  switch(topic->topicType())
  {
    HE_CASE_PROCESS(Status);
    HE_CASE_PROCESS(Expose);
    HE_CASE_PROCESS(Fd);
  }
  updateMain(topic, data, data.getMemberNames());
}

he::model::metrics::TMetricLabels CDevice::defaultLabels()
{
  return {{"name", m_name}};
}

std::string CDevice::metricHelpSensor(const std::string &itemName, const Json::Value &data)
{
  std::string result = itemName;
  if(data.isMember("class")) { result += ", class: " + data["class"].asString(); }
  if(data.isMember("state")) { result += ", state: " + data["state"].asString(); }
  return result + ", unit: " + (data.isMember("unit") ? data["unit"].asString() : "number");
}

std::string CDevice::metricHelpNumber(const std::string &itemName, const Json::Value &data)
{
  return itemName + ". borders: [" + data["min"].asString() + ":" + data["max"].asString() + "],"
                    " step: " + (data.isMember("step") ? data["step"].asString() : "1") + ","
                    " unit: " + (data.isMember("unit") ? data["unit"].asString() : "number");

  // return std::format("{}. borders: [{}:{}], step: {}, unit: {}",
  //                    itemName,
  //                    data["min"].asString(),
  //                    data["max"].asString(),
  //                    data.isMember("step") ? data["step"].asString() : "1",
  //                    data["unit"].asString());
}

void CDevice::updateStatus(const he::mqtt::CTopic *topic, const Json::Value &data)
{
  HE_LOG_VERBOSE("Updating device " << m_name << " status");
  m_networkAddress    = data["networkAddress"]  .asUInt();
  m_modelName         = data["modelName"]       .asString();
  m_manufacturerName  = data["manufacturerName"].asString();
  m_description       = data["description"]     .asString();
  m_role              = to_role        (data["logicalType"]);
  m_powerSource       = to_power_source(data["powerSource"]);

  updateMain(topic, data, {"active", "cloud", "discovery", "supported", "interviewFinished"});

  HE_ST.metrics().set(he::model::metrics::CMetricName::metricName("info"),
              HE_P_LABELS
              (
                HE_P_LABEL (name             ,m_name                          ),
                HE_P_LABEL (network_address  ,std::to_string(m_networkAddress)),
                HE_P_JLABEL(firmware         ,data, firmware                  ),
                HE_P_JLABEL(manufacturer_code,data, manufacturerCode          ),
                HE_P_JLABEL(version          ,data, version                   ),
                HE_P_JLABEL(ieee_address     ,data, ieeeAddress               )
              ),
              he::model::metrics::CMetricValue(1));
  if(data.isMember("neighbors"))
  {
    // HE_LOG_DBG("Updating neighbors");
    for(const Json::Value &neighbor : data["neighbors"])
    {
      const device::CDevice *toDev   = HE_ST.devices().get(neighbor["networkAddress"].asUInt());
      if(toDev)
      {
        HE_ST.metrics().set(  he::model::metrics::CMetricName::metricName("neighborLinkQuality", topic->topicType()),
                              HE_P_LABELS
                              (
                                HE_P_LABEL(from_name           ,m_name                                     ),
                                HE_P_LABEL(to_name             ,toDev->name()                              ),
                                HE_P_LABEL(from_network_address,std::to_string(m_networkAddress)           ),
                                HE_P_LABEL(to_network_address  ,std::to_string(toDev->networkAddress())    ),
                                HE_P_LABEL(from_role           ,he::model::device::to_string(m_role)       ),
                                HE_P_LABEL(to_role             ,he::model::device::to_string(toDev->role()))
                              ),
                              he::model::metrics::CMetricValue(neighbor["linkQuality"]));
      }
    }
  }
}

void CDevice::updateExpose(const he::mqtt::CTopic *topic, const Json::Value &data)
{
  HE_LOG_VERBOSE("Updating device " << m_name << " expose");
  // HE_LOG_DBG(data.toStyledString());
  for(const std::string &key : data.getMemberNames())
  {
    HE_LOG_DBG("Expose key: " << key);
    HE_LOG_DBG(key << " items: " << data[key]["items"].toStyledString());
    for(const Json::Value &item : data[key]["items"])
    {
      const std::string itemKey = item.asString();
      if (data[key].isMember("options") && data[key]["options"].isMember(itemKey))
      {
        HE_LOG_DBG(key << " item " << itemKey << " options: " << data[key]["options"][itemKey].toStyledString());
        if(data[key]["options"][itemKey].isObject())
        {
          if(data[key]["options"][itemKey].isMember("type"))
          {
            std::string itemKeyType = data[key]["options"][itemKey]["type"].asString();
            if(itemKeyType == "binary" || itemKeyType == "toggle")
            {
              HE_ST.metrics().add<he::model::metrics::CMetricPartial>(he::model::metrics::CMetricName::metricName(itemKey, he::mqtt::ETopic::tFd),
                                                                      "Zigbee device " + itemKey + " status, boolean",
                                                                      "gauge");
              continue;
            }
            if(itemKeyType == "sensor")
            {
              // HE_LOG_DBG(key << " sensor " << itemKey << " options: " << data[key]["options"][itemKey].toStyledString());
              HE_ST.metrics().add<he::model::metrics::CMetricPartial>(he::model::metrics::CMetricName::metricName(itemKey, he::mqtt::ETopic::tFd),
                                                                      metricHelpSensor(itemKey, data[key]["options"][itemKey]),
                                                                      (data[key]["options"][itemKey].isMember("state") && data[key]["options"][itemKey]["state"].asString() == "total_increasing") ? "counter" : "gauge");
              continue;
            }
            if(itemKeyType == "number"                       &&
               data[key]["options"][itemKey].isMember("max") &&
               data[key]["options"][itemKey].isMember("min"))
            {
              // HE_LOG_DBG(key << " sensor " << itemKey << " options: " << data[key]["options"][itemKey].toStyledString());
              HE_ST.metrics().add<he::model::metrics::CMetricPartial>(he::model::metrics::CMetricName::metricName(itemKey, he::mqtt::ETopic::tFd),
                                                                      metricHelpNumber(itemKey, data[key]["options"][itemKey]),
                                                                      "gauge");
              continue;
            }
            if(itemKeyType == "select" ||
               (
                itemKeyType == "sensor" &&
                data[key]["options"][itemKey].isMember("enum")
               ))
            {
              HE_ST.metrics().add<he::model::metrics::CMetricPartial>(he::model::metrics::CMetricName::metricName(itemKey, he::mqtt::ETopic::tFd),
                                                                      HE_ST.metrics().exposeEnumerationSetAndHelp(itemKey, data[key]["options"][itemKey]),
                                                                      "gauge");
            }
          }
          else
          {
            if(itemKey == "light")
            {
              HE_ST.metrics().add<he::model::metrics::CMetricPartial>(he::model::metrics::CMetricName::metricName("status", he::mqtt::ETopic::tFd),
                                                                        "Switch status",
                                                                        "gauge");
              HE_ST.metrics().add<he::model::metrics::CMetricPartial>(he::model::metrics::CMetricName::metricName("level", he::mqtt::ETopic::tFd),
                                                                        "Switch status",
                                                                        "gauge");
              continue;
            }
          }
        }
      }
      else
      {
        // std::string item = data[key]["options"][itemKey].asString();
        if(itemKey == "switch")
        {
          HE_ST.metrics().add<he::model::metrics::CMetricPartial>(he::model::metrics::CMetricName::metricName("status", he::mqtt::ETopic::tFd),
                                                                    "Switch status",
                                                                    "gauge");
          continue;
        }
      }
    }
  }
}

void CDevice::updateFd(const he::mqtt::CTopic *topic, const Json::Value &data)
{
  HE_ST.metrics().increment(he::model::metrics::CMetricName::metricName("incoming_device_updates"), defaultLabels());
  updateMain(topic, data, data.getMemberNames());
}

void CDevice::updateMain(const he::mqtt::CTopic *topic, const Json::Value &data, const std::vector<std::string> &keys)
{
  HE_LOG_VERBOSE("Updating device " << m_name << " " << topic->topic());
  for(const std::string &key : keys)
  {
    if(key == "color") { continue; }
    if(data[key].isObject() || data[key].isArray()) { continue; }
    HE_LOG_DBG(topic->topic() << " key: " << key);
    he::model::metrics::TMetricLabels labels = defaultLabels();
    if(!topic->deviceEndpoint().empty()) { labels["endpoint"] = topic->deviceEndpoint(); }
    he::model::metrics::CMetricValue value = HE_ST.metrics().isExposeEnumeration(key) ?  he::model::metrics::CMetricValue(HE_ST.metrics().exposeEnumerationIndex(key, data[key].asString())) :  he::model::metrics::CMetricValue(data[key]);
    std::string metricName = he::model::metrics::CMetricName::metricName(key, topic->topicType());
    switch(HE_ST.metrics().type(metricName))
    {
      case he::model::metrics::EMetricType::mtPartial:
      case he::model::metrics::EMetricType::mtStatic:
      {
        HE_ST.metrics().set(metricName, labels, value);
      } break;
      case he::model::metrics::EMetricType::mtIncremental:
      {
        HE_ST.metrics().increment(metricName, labels, value.value());
      } break;
    }

  }
}

}
}
}
