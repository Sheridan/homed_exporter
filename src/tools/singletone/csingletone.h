#pragma once
#include "tools/singletone/singletone.h"
#include "configuration/ccmdoptions.h"
#include "configuration/cconfiguration.h"
#include "http/chttp.h"
#include "model/chomed.h"
#include "model/device/cdevices.h"
#include "model/metrics/cmetrics.h"

namespace he
{

class CSingleTone
{
  HE_SIMPLE_SINGLTON_DECLARATION(he::configuration::CCmdLine      , cmdLine)
  HE_SIMPLE_SINGLTON_DECLARATION(he::configuration::CConfiguration, config )
  HE_SIMPLE_SINGLTON_DECLARATION(he::http::CHttp                  , http   )
  HE_SIMPLE_SINGLTON_DECLARATION(he::model::CHomed                , homed  )
  HE_SIMPLE_SINGLTON_DECLARATION(he::model::device::CDevices      , devices)
  HE_SIMPLE_SINGLTON_DECLARATION(he::model::metrics::CMetrics     , metrics)
public:
  static CSingleTone &instance();
  void destruct();

private:
  CSingleTone();
  ~CSingleTone();
};

}
