#include "mqtt/cmqttclient.h"
#include <iostream>
#include "st.h"


int main(int argc, char* argv[])
{
  HE_ST.cmdLine().addOption       <bool>("-v", "--verbose", "Verbose output"      , false);
  HE_ST.cmdLine().addOption<std::string>("-c", "--config" , "Config file location", "/etc/homed-exporter.json");
  if(!HE_ST.cmdLine().parse(argc, argv)) { return 1; };

  HE_ST.config().load(HE_ST.cmdLine().option<std::string>("config"));

  std::string url = "tcp://" + HE_ST.config().mqttServer() + ":" + std::to_string(HE_ST.config().mqttPort());
  he::mqtt::СMqttClient mqttClient(url,
                                  "homed_exporter",
                                  HE_ST.config().mqttUser(),
                                  HE_ST.config().mqttPassword());
  mqttClient.subscribe();

  HE_ST.http().listen(HE_ST.config().exporterAddress(), HE_ST.config().exporterPort());

  return 0;
}
