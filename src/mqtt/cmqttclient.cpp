#include "st.h"
#include "mqtt/cmqttclient.h"
#include <iostream>

namespace he
{
namespace mqtt
{

СMqttClient::СMqttClient(const std::string& serverAddress, const std::string& clientId,
                         const std::string& username     , const std::string& password)
    : m_client(serverAddress, clientId), m_connectionOptions()
{

  m_connectionOptions.set_user_name(username);
  m_connectionOptions.set_password(password);

  m_client.set_callback(m_callback);

  try
  {
    HE_LOG_NFO("Connecting to the MQTT server...");
    m_client.connect(m_connectionOptions)->wait();
    HE_LOG_NFO("MQTT connected.");
  }
  catch (const ::mqtt::exception& e)
  {
    HE_ABORT("MQTT connection error: " << e.what());
  }
}

СMqttClient::~СMqttClient()
{
  try
  {
    m_client.disconnect()->wait();
  }
  catch (const ::mqtt::exception& e)
  {
    HE_LOG_ERR("Error during MQTT disconnection: " << e.what());
  }
}

void СMqttClient::subscribe()
{
  try
  {
    std::string topic = HE_ST.config().mqttHomedTopic() + "/#";
    m_client.subscribe(topic, 1)->wait();
    HE_LOG_NFO("Subscribed to topic '" << topic << "'.");
  }
  catch (const ::mqtt::exception& e)
  {
    HE_ABORT("MQTT subscription error: " << e.what());
  }
}

}
}
