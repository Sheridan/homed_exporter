#include "model/metrics/cmetricvalue.h"
#include "cmetricvalue.h"
// #include <format>

namespace he
{
namespace model
{
namespace metrics
{

CMetricValue::CMetricValue()                                                      : m_value(0        ), m_timestamp(he::utils::now()), m_isNaN(true     ) {}
CMetricValue::CMetricValue(const TMetricValue &v)                                 : m_value(v        ), m_timestamp(he::utils::now()), m_isNaN(false    ) {}
CMetricValue::CMetricValue(const TMetricValue &v, const he::utils::TTimestamp &t) : m_value(v        ), m_timestamp(t               ), m_isNaN(false    ) {}
CMetricValue::CMetricValue(const CMetricValue &v)                                 : m_value(v.value()), m_timestamp(v.timestamp()   ), m_isNaN(v.isNaN()) {}

CMetricValue::CMetricValue(const Json::Value &v)
{
  if(v.isNull())
  {
    reset();
  }
  else
  {
    if(v.isString())
    {
      set(stringToValue(v.asString()));
    }
    else
    {
      set(v.asDouble());
    }
  }
}

CMetricValue::~CMetricValue() {}

void CMetricValue::set(const TMetricValue &v)
{
  m_isNaN = false;
  m_timestamp = he::utils::now();
  m_value = v;
}

void CMetricValue::reset()
{
  m_isNaN     = true;
  m_timestamp = 0;
  m_value     = 0;
}

void CMetricValue::increment(const TMetricValue &v)
{
  m_isNaN = false;
  m_timestamp = he::utils::now();
  m_value += v;
}

std::string CMetricValue::asMetric()
{
  // return std::format("{} {}", m_isNaN ? "NaN" : std::to_string(m_value), m_timestamp);
  return (m_isNaN ? "NaN" : std::to_string(m_value)) + " " + std::to_string(m_timestamp);
}

CMetricValue &CMetricValue::operator=(const CMetricValue &other)
{
  if (this == &other) { return *this; }
  m_isNaN     = other.isNaN();
  m_value     = other.value();
  m_timestamp = other.timestamp();
  return *this;
}

TMetricValue CMetricValue::stringToValue(const std::string &input)
{
  if(input == "on"     || input == "off"   ) { return input == "on"     ? 1 : 0; }
  if(input == "online" || input == "online") { return input == "online" ? 1 : 0; }
  return 0;
}
}
}
}
