#pragma once
#include "model/metrics/cmetricstatic.h"
#include <queue>
namespace he
{
namespace model
{
namespace metrics
{

class CMetricPartial : public CMetric
{
public:
           CMetricPartial();
  virtual ~CMetricPartial();

  void         set      (const CMetricValue &value) final;
  CMetricValue get      (                         ) final;
  bool         empty    (                         ) final;
  void         increment(const TMetricValue &value) final;

private:
  std::queue<CMetricValue> m_metric;
};

}
}
}
