#pragma once
#include <string>
#include "model/metrics/cmetric.h"
namespace he
{
namespace model
{
namespace metrics
{

class CMetricStatic : public CMetric
{
public:
           CMetricStatic();
  virtual ~CMetricStatic();

  virtual void         set      (const CMetricValue &value) final;
  virtual CMetricValue get      (                         ) final;
  virtual bool         empty    (                         ) final;
  virtual void         increment(const TMetricValue &value) final;

private:
  CMetricValue m_metric;
};

}
}
}
