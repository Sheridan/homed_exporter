#pragma once
#include <limits>
#include "model/metrics/cmetricstatic.h"

// #define HE_HOMED_METRIC_PREFIX "homed_"
// #define HE_HOMED_METRIC_NAME(_name) HE_HOMED_METRIC_PREFIX _name
// #define HE_HOMED_ZIGBEE_DEVICE_METRIC_NAME(_name) HE_HOMED_METRIC_PREFIX "zigbee_device_" _name


#define HE_P_LABEL(_name,_value) {#_name,_value}
#define HE_P_JLABEL(_name,_json,_value) {#_name, (_json.isMember(#_value) ? _json[#_value].asString() : "unknown")}
#define HE_P_LABELS(...) {__VA_ARGS__}
// #define HE_P_LABELS_DEFAULT(_name) HE_P_LABELS(HE_P_LABEL(name,_name))
// #define HE_P_JLABELS_DEFAULT(_json) HE_P_LABELS(HE_P_JLABEL(name,_json,name))

#define HE_P_JVALUE(_json,_name) he::model::metrics::CMetricValue(_json[#_name])


// #define HE_MERIC_SET(_name,_labels,_json,_object) \
//   if(_json.isMember(#_name)) \
//   { \
//     _object->set(_labels, he::model::metrics::CMetricValue(_json[#_name])); \
//   }

// #define HE_MERIC_SIMPLE_SET(_name,_device_name,_json,_object) HE_MERIC_SET(_name,HE_P_LABELS_DEFAULT(_device_name),_json,_object)
