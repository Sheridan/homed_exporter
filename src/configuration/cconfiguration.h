#pragma once
#include <string>
#include <jsoncpp/json/json.h>

namespace he
{
namespace configuration
{

#define HE_CONF_OPTION(_name,_type,_1key,_2key,_convert,_default) \
_type _name() \
{ \
  if(m_json.isMember(_1key) && m_json[_1key].isMember(_2key)) \
  { \
    return m_json[_1key][_2key]._convert(); \
  } \
  return _default; \
}

class CConfiguration
{
public:
  CConfiguration();
  virtual ~CConfiguration();

  void load(const std::string &filename);

  HE_CONF_OPTION(mqttServer     , std::string, "mqtt", "server"          , asString, "localhost");
  HE_CONF_OPTION(mqttPort       , int        , "mqtt", "port"            , asInt   , 1833       );
  HE_CONF_OPTION(mqttUser       , std::string, "mqtt", "user"            , asString, "exporter" );
  HE_CONF_OPTION(mqttPassword   , std::string, "mqtt", "password"        , asString, "exporter" );
  HE_CONF_OPTION(mqttHomedTopic , std::string, "mqtt", "homed_root_topic", asString, "homed"    );

  HE_CONF_OPTION(exporterAddress, std::string, "exporter", "address"     , asString, "127.0.0.1");
  HE_CONF_OPTION(exporterPort   , int        , "exporter", "port"        , asInt   , 9111       );

  bool labelsExists(const std::string &name);
  const Json::Value &labels(const std::string &name);

  bool ignoresExists(const std::string &name);
  const Json::Value &ignores(const std::string &name);

private:
  Json::Value m_json;
  bool listExists(const std::string &containerName, const std::string &listName, const std::string &name);
  const Json::Value &list(const std::string &containerName, const std::string &listName, const std::string &name);

};

}
}
