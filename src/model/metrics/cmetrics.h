#pragma once
#include "model/metrics/cmetricbulk.h"
#include "model/metrics/cmetricvalue.h"
#include "log.h"
#include <map>
namespace he
{
namespace model
{
namespace metrics
{

typedef std::map<std::string, CMetricBulkBase *> TBulkMetrics;
class CMetrics
{
public:
  CMetrics();
  virtual ~CMetrics();

  template <typename T>
  void add(const std::string &name, const std::string &help, const std::string &type)
  {
    if(ensureMetricExists(name)) { return; }
    HE_LOG_NFO("Found new metric: " << name << ". Metric help: " << help);
    m_metrics[name] = new CMetricBulk<T>(help, type);
  }

  void        set      (const std::string &name, const TMetricLabels &labels, const CMetricValue &value);
  void        increment(const std::string &name, const TMetricLabels &labels, const TMetricValue &value = 1);
  EMetricType type     (const std::string &name);
  bool        empty    (const std::string &name);
  bool        empty    (                       );
  void        clear    (const std::string &name);


  std::string metrics();
  std::string list();

  he::model::metrics::TMetricValue exposeEnumerationIndex(const std::string &metricName, const std::string &metricValue);
  std::string exposeEnumerationSetAndHelp(const std::string &metricName, const Json::Value &data);
  bool isExposeEnumeration(const std::string &metricName);

private:
  TBulkMetrics m_metrics;
  std::map<std::string, std::map<std::string, unsigned short>> m_exposeEnumerates;

  bool ensureMetricExists(const std::string &name);
};

}
}
}
