#pragma once
#include "model/metrics/types/cmetric.h"

namespace he
{
namespace model
{
namespace metrics
{

class CMetricIncremental : public CMetric
{
public:
  static constexpr EMetricType type() { return EMetricType::mtIncremental; }

           CMetricIncremental();
  virtual ~CMetricIncremental();

  void         set      (const CMetricValue &value) final;
  CMetricValue get      (                         ) final;
  bool         empty    (                         ) final;
  void         increment(const TMetricValue &value) final;


private:
  CMetricValue m_metric;
  bool is_empty;
};


}
}
}
