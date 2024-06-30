#include "model/metrics/cmetrics.h"
#include "model/metrics/cmetricname.h"
#include "cmetrics.h"
#include "st.h"

namespace he
{
namespace model
{
namespace metrics
{

CMetrics:: CMetrics()
{
  HE_LOG_DBG("CMetrics initialized");
  add<he::model::metrics::CMetricPartial>(he::model::metrics::CMetricName::metricName("active"             ,he::mqtt::ETopic::tStatus ), "Device active (bool)", "gauge");
  add<he::model::metrics::CMetricPartial>(he::model::metrics::CMetricName::metricName("cloud"              ,he::mqtt::ETopic::tStatus ), "Device in cloud (bool)", "gauge");
  add<he::model::metrics::CMetricPartial>(he::model::metrics::CMetricName::metricName("discovery"          ,he::mqtt::ETopic::tStatus ), "Device discovery status (bool)", "gauge");
  add<he::model::metrics::CMetricPartial>(he::model::metrics::CMetricName::metricName("interviewFinished"  ,he::mqtt::ETopic::tStatus ), "Interview with device is finished (bool)", "gauge");
  add<he::model::metrics::CMetricPartial>(he::model::metrics::CMetricName::metricName("supported"          ,he::mqtt::ETopic::tStatus ), "Device is supported (bool)", "gauge");
  add<he::model::metrics::CMetricPartial>(he::model::metrics::CMetricName::metricName("neighborLinkQuality",he::mqtt::ETopic::tStatus ), "Link quality between device and their neightbors", "gauge");
  add<he::model::metrics::CMetricPartial>(he::model::metrics::CMetricName::metricName("linkQuality"        , he::mqtt::ETopic::tFd    ), "Link quality", "gauge");
  add<he::model::metrics::CMetricPartial>(he::model::metrics::CMetricName::metricName("lastSeen"           , he::mqtt::ETopic::tDevice), "Device last seen time", "gauge");
  add<he::model::metrics::CMetricPartial>(he::model::metrics::CMetricName::metricName("status"             , he::mqtt::ETopic::tDevice), "Device online status", "gauge");

  add<he::model::metrics::CMetricStatic> (he::model::metrics::CMetricName::metricName("info")   , "Device info in tags", "gauge");

  add<he::model::metrics::CMetricIncremental>(he::model::metrics::CMetricName::metricName("incoming_device_updates"), "Device updates. Incomed data from device between metric reads", "gauge");
}

CMetrics::~CMetrics()
{
  for (auto &pair : m_metrics)
  {
    delete pair.second;
  }
}

void CMetrics::set(const std::string &name, const TMetricLabels &labels, const CMetricValue &value)
{
  if(!ensureMetricExists(name)) { return; }
  m_metrics[name]->set(labels, value);
}

void CMetrics::increment(const std::string &name, const TMetricLabels &labels, const TMetricValue &value)
{
  if(!ensureMetricExists(name)) { return; }
  m_metrics[name]->increment(labels, value);
}

bool CMetrics::empty(const std::string &name)
{
  return !(m_metrics.contains(name) && !m_metrics[name]->empty());
}

bool CMetrics::empty()
{
  if(m_metrics.empty()) { return true; }
  for (const auto &pair : m_metrics)
  {
    if(!pair.second->empty()) { return false; };
  }
  return true;
}

void CMetrics::clear(const std::string &name)
{
  if(!ensureMetricExists(name)) { return; }
  m_metrics[name]->clear();
}

std::string CMetrics::metrics()
{
  std::string result = "";
  for (const auto &pair : m_metrics)
  {
    if(!pair.second->empty()) { result += pair.second->metrics(pair.first); };
  }
  return result;
}

std::string CMetrics::list()
{
  std::string result = "";
  for (const auto &pair : m_metrics)
  {
    result += pair.second->info(pair.first);
  }
  return result;
}

bool CMetrics::ensureMetricExists(const std::string &name)
{
  if(m_metrics.contains(name)) { return true; }
  HE_LOG_WRN("Metric " << name << " doesn't exist yet...");
  return false;
}

he::model::metrics::TMetricValue CMetrics::exposeEnumerationIndex(const std::string &metricName, const std::string &metricValue)
{
  return he::model::metrics::TMetricValue(m_exposeEnumerates[metricName][metricValue]);
}

std::string CMetrics::exposeEnumerationSetAndHelp(const std::string &metricName, const Json::Value &data)
{
  std::string help = "metricName, type: " + data["type"].asString() + " enum: [";
  unsigned short index = 0;
  for(const Json::Value &enumerated : data["enum"])
  {
    if(enumerated.isString())
    {
      m_exposeEnumerates[metricName][enumerated.asString()] = index;
      help += std::to_string(index) + ":" + enumerated.asString() + ",";
    }
    else
    {
      for(const std::string &enumeratedKey : enumerated.getMemberNames())
      {
        m_exposeEnumerates[metricName][enumerated[enumeratedKey].asString()] = std::stoi(enumeratedKey);
        help += enumeratedKey + ":" + enumerated[enumeratedKey].asString() + ",";
      }
    }
    index++;
  }
  return help.substr(0, help.size()-1) + "]";
}

bool CMetrics::isExposeEnumeration(const std::string &metricName)
{
  return m_exposeEnumerates.contains(metricName);
}

}
}
}
