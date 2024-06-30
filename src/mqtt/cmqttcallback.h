#pragma once
#include <mqtt/async_client.h>

namespace he
{
namespace mqtt
{


class CMqttCallback : public virtual ::mqtt::callback
{
public:
  CMqttCallback();
  ~CMqttCallback();
  void message_arrived(::mqtt::const_message_ptr msg) override;

};

}
}
