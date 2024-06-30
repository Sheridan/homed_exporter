#include "mqtt/cpayload.h"
#include "st.h"
namespace he
{
namespace mqtt
{

CPayload::CPayload()
{
  m_builder = new Json::CharReaderBuilder();
  m_reader = m_builder->newCharReader();
}

bool CPayload::validate(const std::string& payload)
{
  if(payload.empty()) { return false; }
  return true;
}

bool CPayload::parse(const std::string& payload)
{
  // HE_LOG_DBG(payload);
  if(!validate(payload)) { return false; }
  JSONCPP_STRING err;
  if (!m_reader->parse(payload.c_str(), payload.c_str() + payload.length(), &m_root, &err))
  {
    HE_LOG_WRN("MQTT payload parse error: " << err);
    return false;
  }
  // HE_LOG_DBG(m_root.toStyledString());
  return true;
}

CPayload::~CPayload()
{
  delete m_builder;
  delete m_reader;
}

}
}
