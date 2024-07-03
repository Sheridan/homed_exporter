#include "model/metrics/types/cmetricincremental.h"
#include <limits>
#include "st.h"

namespace he
{
namespace model
{
namespace metrics
{

CMetricIncremental:: CMetricIncremental() : CMetric(), m_metric(CMetricValue()), is_empty(true) { }
CMetricIncremental::~CMetricIncremental()                                                       { }

void CMetricIncremental::set(const CMetricValue &value)
{
  m_metric = value;
  is_empty = false;
}

CMetricValue CMetricIncremental::get()
{
  is_empty = true;
  return m_metric;
}

bool CMetricIncremental::empty()
{
  return is_empty;
}

void CMetricIncremental::increment(const TMetricValue &value)
{
  if (value > 0 && std::numeric_limits<TMetricValue>::max() - m_metric.value() < value)
  {
    m_metric.set(0);
  }
  m_metric.increment(value);
  is_empty = false;
}

}
}
}
