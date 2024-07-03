#include "model/metrics/types/cmetricstatic.h"


namespace he
{
namespace model
{
namespace metrics
{

CMetricStatic:: CMetricStatic() : CMetric() {}
CMetricStatic::~CMetricStatic()             {}

void CMetricStatic::set(const CMetricValue &value)
{
  m_metric = CMetricValue(static_cast<TMetricValue>(1));
}

CMetricValue CMetricStatic::get()
{
  m_metric.reset();
  return CMetricValue(static_cast<TMetricValue>(1));
}

bool CMetricStatic::empty()
{
  return m_metric.timestamp() == 0;
}

void CMetricStatic::increment(const TMetricValue &value)
{}

}
}
}
