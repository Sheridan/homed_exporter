#pragma once
#include <string>
#include <jsoncpp/json/json.h>
#include "utils/datetime.h"
#include "defines/class_helpers.h"
namespace he
{
namespace model
{
namespace metrics
{

typedef double TMetricValue;

class CMetricValue
{
public:
  CMetricValue();
  explicit CMetricValue(const TMetricValue &v);
  CMetricValue(const TMetricValue &v, const he::utils::TTimestamp &t);
  CMetricValue(const CMetricValue &v);
  explicit CMetricValue(const Json::Value &v);
  virtual ~CMetricValue();


  HE_SIMPLE_GETTER(bool                 ,isNaN    );
  HE_SIMPLE_GETTER(TMetricValue         ,value    );
  HE_SIMPLE_GETTER(he::utils::TTimestamp,timestamp);

  void set(const TMetricValue &v);
  void reset();
  void increment(const TMetricValue &v = 1);
  std::string asMetric();

  CMetricValue &operator=(const CMetricValue &other);

private:
  TMetricValue          m_value;
  he::utils::TTimestamp m_timestamp;
  bool                  m_isNaN;

  TMetricValue stringToValue(const std::string &input);
};


}
}
}
