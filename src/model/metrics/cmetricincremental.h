#pragma once
#include "model/metrics/cmetricstatic.h"

namespace he
{
namespace model
{
namespace metrics
{

class CMetricIncremental : public CMetric
{
public:
           CMetricIncremental();
  virtual ~CMetricIncremental();

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
