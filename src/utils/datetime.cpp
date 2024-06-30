#include "datetime.h"
#include <chrono>
// #include <ctime>

namespace he
{
namespace utils
{

TTimestamp now()
{
  auto now = std::chrono::system_clock::now();
  auto timestamp_duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
  return timestamp_duration.count();
}

}
}
