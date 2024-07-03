#pragma once
#include <string>
#include "model/metrics/types/cmetric.h"
namespace he
{
namespace model
{
namespace metrics
{

class CMetricStatic : public CMetric
{
public:
  static constexpr EMetricType type() { return EMetricType::mtStatic; }

           CMetricStatic();
  virtual ~CMetricStatic();

  void         set      (const CMetricValue &value) final;
  CMetricValue get      (                         ) final;
  bool         empty    (                         ) final;
  void         increment(const TMetricValue &value) final;

private:
  CMetricValue m_metric;
};

}
}
}
