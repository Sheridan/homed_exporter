#pragma once
#include "mqtt/cmqttcallback.h"
#include <mqtt/async_client.h>
#include <string>

namespace he
{
namespace mqtt
{

class СMqttClient
{
public:
  СMqttClient(const std::string& serverAddress, const std::string& clientId,
              const std::string& username     , const std::string& password);
  ~СMqttClient();

  void subscribe();

private:
  ::mqtt::async_client m_client;
  ::mqtt::connect_options m_connectionOptions;
  CMqttCallback m_callback;
};

}
}
