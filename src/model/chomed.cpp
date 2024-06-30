#include "model/chomed.h"
#include "defines/metrics.h"
#include "model/metrics/cmetricname.h"
#include "st.h"

// #include <format>

namespace he
{
namespace model
{

CHomed:: CHomed()
{
  HE_LOG_DBG("CHomed initialized");

}
CHomed::~CHomed() {}

#define HE_CASE_PROCESS(_type) \
    case he::mqtt::ETopic::t##_type: \
    { \
      /* HE_LOG_DBG("Processing " #_type); */ \
      process##_type(topic, data); \
    }; break;


void CHomed::update(const he::mqtt::CTopic *topic, const Json::Value &data)
{
  std::lock_guard<std::mutex> lock(m_dataMutex);
  if(topic->topicType() != he::mqtt::ETopic::tStatus && HE_ST.devices().empty()) { return; }

  HE_LOG_DBG("Updating metrics");
  switch(topic->topicType())
  {
    HE_CASE_PROCESS(Status);
    HE_CASE_PROCESS(Fd);
    HE_CASE_PROCESS(Device);
    HE_CASE_PROCESS(Expose);
  }
  HE_LOG_DBG("Updating metrics done");
}

void CHomed::processStatus(const he::mqtt::CTopic *topic, const Json::Value &data)
{
  if(data.isMember("devices"))
  {
    HE_ST.devices().update(topic, data["devices"]);
  }
}

void CHomed::processFd(const he::mqtt::CTopic *topic, const Json::Value &data)
{
  HE_ST.devices().update(topic, data);
}

void CHomed::processDevice(const he::mqtt::CTopic *topic, const Json::Value &data)
{
  HE_ST.devices().update(topic, data);
}

void CHomed::processExpose(const he::mqtt::CTopic *topic, const Json::Value &data)
{
  HE_ST.devices().update(topic, data);
}

std::string CHomed::metrics()
{
  std::lock_guard<std::mutex> lock(m_dataMutex);
  if(HE_ST.devices().empty() || HE_ST.metrics().empty()) { return ""; }
  HE_LOG_DBG("Render metrics");
  return HE_ST.metrics().metrics();
}




}
}
