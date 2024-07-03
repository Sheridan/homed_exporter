#pragma once
#include <string>
#include "model/metrics/cmetricvalue.h"
namespace he
{
namespace model
{
namespace metrics
{

enum EMetricType
{
  mtStatic,
  mtPartial,
  mtIncremental,
  mtUnknown
};
class CMetric
{
public:
           CMetric();
  virtual ~CMetric();

  virtual void         set      (const CMetricValue &value) = 0;
  virtual CMetricValue get      (                         ) = 0;
  virtual bool         empty    (                         ) = 0;
  virtual void         increment(const TMetricValue &value) = 0;

};

}
}
}
