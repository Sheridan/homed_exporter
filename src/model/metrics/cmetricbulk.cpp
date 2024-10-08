
#include "model/metrics/cmetricbulk.h"
#include "model/device/cdevice.h"
#include "utils/string.h"
#include <jsoncpp/json/json.h>
// #include <format>
#include "st.h"

namespace he
{
namespace model
{
namespace metrics
{

template <typename TMetricType>
CMetricBulk<TMetricType>::CMetricBulk(const std::string &help, const std::string &type)
  : m_help(help), m_type(type)
{}

template <typename TMetricType>
CMetricBulk<TMetricType>::~CMetricBulk()
{
  clear();
}

template <typename TMetricType>
void CMetricBulk<TMetricType>::presentMetric(const TMetricLabels &labels)
{
  if(!m_metrics.contains(labels))
  {
    m_metrics[labels] = new TMetricType();
  }
}

template <typename TMetricType>
void CMetricBulk<TMetricType>::set(const TMetricLabels &labels, const CMetricValue &value)
{
  // HE_LOG_NFO << "CMetricBulk:set " << m_name << std::endl;
  presentMetric(labels);
  m_metrics[labels]->set(value);
}

template <typename TMetricType>
void CMetricBulk<TMetricType>::increment(const TMetricLabels &labels, const TMetricValue &value)
{
  // HE_LOG_NFO << "CMetricBulk:increment " << m_name << std::endl;
  presentMetric(labels);
  m_metrics[labels]->increment(value);
}

template <typename TMetricType>
bool CMetricBulk<TMetricType>::empty()
{
  for (auto &pair : m_metrics)
  {
    if(!pair.second->empty()) { return false; }
  }
  return true;
}

template <typename TMetricType>
void CMetricBulk<TMetricType>::clear()
{
  // HE_LOG_NFO << "CMetricBulk:clear " << m_name << std::endl;
  for (auto &pair : m_metrics)
  {
    delete pair.second;
  }
  m_metrics.clear();
}

template <typename TMetricType>
std::string CMetricBulk<TMetricType>::metrics(const std::string &metricName)
{
  // HE_LOG_NFO << "CMetricBulk:metrics " << metricName << std::endl;
  std::string result = "";
  for (const auto &pair : m_metrics)
  {
    while(!pair.second->empty())
    {
      CMetricValue metric = pair.second->get();
      if(!metric.isNaN())
      {
        TMetricLabels labels(pair.first);
        // ---------- labels ----------------
        std::string deviceName = labelValue(pair.first, "name");
        if(!deviceName.empty() && HE_ST.devices().exists(deviceName))
        {
          const he::model::device::CDevice *device = HE_ST.devices().get(deviceName);
          labels["model_name"]          = device->modelName();
          labels["manufacturer_mame"]   = device->manufacturerName();
          labels["description"]         = device->description();
          labels["role"]                = he::model::device::to_string(device->role());
          labels["power_source"]        = he::model::device::to_string(device->powerSource());
        }
        if(!deviceName.empty() && HE_ST.config().labelsExists("devices"))
        {
          const Json::Value &jLabels = HE_ST.config().labels("devices");
          if(jLabels.isMember(deviceName))
          {
            for(const Json::Value &label : jLabels[deviceName])
            {
              for(const std::string &key : label.getMemberNames())
              {
                labels[key] = label[key].asString();
              }
            }
          }
        }
        // ---------- endpoint labels ----------------
        std::string deviceEndpoint = labelValue(pair.first, "endpoint");
        if(!deviceEndpoint.empty() && HE_ST.config().labelsExists("endpoints"))
        {
          const Json::Value &jLabels = HE_ST.config().labels("endpoints");
          if(jLabels.isMember(deviceName) && jLabels[deviceName].isMember(deviceEndpoint))
          {
            for(const Json::Value &label : jLabels[deviceName][deviceEndpoint])
            {
              for(const std::string &key : label.getMemberNames())
              {
                labels[key] = label[key].asString();
              }
            }
          }
        }
        // result += std::format("{}{{{}}} {}\n", metricName, makeLabels(labels), metric.asMetric());
        result += metricName + "{" + makeLabels(labels) + "} " + metric.asMetric() + "\n";
      }
    }
  }
  // clear();
  if(!result.empty())
  {
    // return std::format("# HELP {} {}\n# TYPE {} {}\n", metricName, m_help, metricName, m_type) + result;
    return "# HELP " + metricName + " " + m_help + "\n# TYPE " + metricName + " " + m_type + "\n" + result;
  }
  return he::utils::empty_string;
}


template<typename TMetricType>
std::string CMetricBulk<TMetricType>::info(const std::string &metricName)
{
  // return std::format("Metric name: {}; Metric type: {}; Metric help: {}\n", metricName, m_type, m_help);
  return "Metric name: " + metricName + "; Metric type: " + m_type + "; Metric help: " + m_help + "\n";
}

template <typename TMetricType>
std::string CMetricBulk<TMetricType>::makeLabels(const TMetricLabels &labels)
{
  std::string result = "";
  for(auto &pair : labels)
  {
    // result += std::format("{}=\"{}\",", pair.first, pair.second);
    result += pair.first + "=\"" + pair.second + "\",";
  }
  return result.substr(0, result.size()-1);
}

template <typename TMetricType>
const std::string &CMetricBulk<TMetricType>::labelValue(const TMetricLabels &labels, const std::string &name)
{
  for(const auto &pair : labels)
  {
    if(pair.first == name) { return pair.second; }
  }
  return he::utils::empty_string;
}



}
}
}

#include "model/metrics/types/cmetricincremental.h"
#include "model/metrics/types/cmetricpartial.h"
#include "model/metrics/types/cmetricstatic.h"
#include "cmetricbulk.h"

namespace he
{
namespace model
{
namespace metrics
{

// template class CMetricBulk<CMetric>;
template class CMetricBulk<CMetricStatic>;
template class CMetricBulk<CMetricPartial>;
template class CMetricBulk<CMetricIncremental>;

}
}
}
