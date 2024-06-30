#pragma once
#include "model/metrics/cmetricstatic.h"
#include <string>
#include <map>

namespace he
{
namespace model
{
namespace metrics
{

typedef std::map<std::string, std::string> TMetricLabels;

class CMetricBulkBase
{
public:
  virtual void set      (const TMetricLabels &labels, const CMetricValue &value) = 0;
  virtual void increment(const TMetricLabels &labels, const TMetricValue &value) = 0;
  virtual bool empty() = 0;
  virtual void clear() = 0;
  virtual std::string metrics() = 0;
  virtual std::string info() = 0;
};

template <typename TMetricType>
class CMetricBulk : public CMetricBulkBase
{
  typedef std::map<TMetricLabels, TMetricType *> TMetrics;

public:
  CMetricBulk(const std::string &name, const std::string &help, const std::string &type);
  ~CMetricBulk();
  void set      (const TMetricLabels &labels, const CMetricValue &value);
  void increment(const TMetricLabels &labels, const TMetricValue &value = 1);
  bool empty();
  void clear();
  std::string metrics();
  std::string info();

private:
  std::string m_name;
  std::string m_help;
  std::string m_type;
  TMetrics m_metrics;

  void presentMetric(const TMetricLabels &labels);
  std::string makeLabels(const TMetricLabels &labels);
  const std::string &labelValue(const TMetricLabels &labels, const std::string &name);
};

}
}
}
