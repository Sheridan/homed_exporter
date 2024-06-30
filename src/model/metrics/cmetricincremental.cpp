#include "model/metrics/cmetricincremental.h"
#include "cmetricincremental.h"
#include "st.h"

namespace he
{
namespace model
{
namespace metrics
{

CMetricIncremental:: CMetricIncremental() : CMetric(), m_metric(CMetricValue()) { }
CMetricIncremental::~CMetricIncremental()                                       { }

void CMetricIncremental::set(const CMetricValue &value)
{
  m_metric = value;
}

CMetricValue CMetricIncremental::get()
{
  // HE_LOG_NFO << "CMetricIncremental:get" << std::endl;
  CMetricValue result(m_metric);
  m_metric.reset();
  return result;
}

bool CMetricIncremental::empty()
{
  // HE_LOG_NFO << "CMetricIncremental:empty" << std::endl;
  return m_metric.timestamp() == 0;
}

void CMetricIncremental::increment(const TMetricValue &value)
{
  m_metric.increment(value);
}

}
}
}
