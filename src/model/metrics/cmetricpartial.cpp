#include "model/metrics/cmetricpartial.h"
#include "cmetricpartial.h"

namespace he
{
namespace model
{
namespace metrics
{

CMetricPartial:: CMetricPartial() : CMetric() {}
CMetricPartial::~CMetricPartial()             {}

void CMetricPartial::set(const CMetricValue &value)
{
  m_metric.push(value);
}

CMetricValue CMetricPartial::get()
{
  CMetricValue result(m_metric.front());
  m_metric.pop();
  return result;
}

bool CMetricPartial::empty()
{
  return m_metric.empty();
}

void CMetricPartial::increment(const TMetricValue &value)
{
  set(CMetricValue(m_metric.front().value() + value));
}

}
}
}
