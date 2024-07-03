#pragma once
#include <jsoncpp/json/json.h>
#include <string>
#include <map>
#include <mutex>

#include "model/metrics/cmetricbulk.h"
#include "model/metrics/types/cmetricincremental.h"
#include "model/metrics/types/cmetricpartial.h"
#include "model/metrics/types/cmetricstatic.h"
#include "mqtt/ctopic.h"

namespace he
{
namespace model
{

typedef metrics::CMetricBulk<metrics::CMetricPartial> TMetricPartial;
typedef metrics::CMetricBulk<metrics::CMetricIncremental> TMetricIncremental;
typedef metrics::CMetricBulk<metrics::CMetricStatic> TMetricStatic;

class CHomed
{

public:
  CHomed();
  ~CHomed();

   void update(const he::mqtt::CTopic *topic, const Json::Value &data);
   std::string metrics();

private:
  std::mutex m_dataMutex;



  void processStatus(const he::mqtt::CTopic *topic, const Json::Value &data);
  void processFd    (const he::mqtt::CTopic *topic, const Json::Value &data);
  void processDevice(const he::mqtt::CTopic *topic, const Json::Value &data);
  void processExpose(const he::mqtt::CTopic *topic, const Json::Value &data);


};

}
}
