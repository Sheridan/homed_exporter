#pragma once
#include "model/metrics/types/cmetric.h"
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
  static constexpr EMetricType type() { return EMetricType::mtPartial; }

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
