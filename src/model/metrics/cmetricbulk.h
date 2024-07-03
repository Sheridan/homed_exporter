#pragma once
#include <string>
#include <map>
#include "model/metrics/types/cmetric.h"

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
  virtual std::string metrics(const std::string &metricName) = 0;
  virtual std::string info(const std::string &metricName) = 0;
  virtual const EMetricType type() = 0;
};

template <typename TMetricType>
class CMetricBulk : public CMetricBulkBase
{
  typedef std::map<TMetricLabels, TMetricType *> TMetrics;

public:

  CMetricBulk(const std::string &help, const std::string &type);
  ~CMetricBulk();
  void set           (const TMetricLabels &labels, const CMetricValue &value)     final;
  void increment     (const TMetricLabels &labels, const TMetricValue &value = 1) final;
  bool empty         (                                                          ) final;
  void clear         (                                                          ) final;
  std::string metrics(const std::string &metricName) final;
  std::string info   (const std::string &metricName) final;

  const EMetricType type() final { return TMetricType::type(); }

private:
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
