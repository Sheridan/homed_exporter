#pragma once
// #include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>
#include "tools/singletone/csingletone.h"


// #define HE_STR_HELPER(x) #x
// #define HE_STR(x) HE_STR_HELPER(x)

#define HE_ST he::CSingleTone::instance()

#include "log.h"

#define HE_LOG_VERBOSE(_what) if(HE_ST.cmdLine().option<bool>("verbose")) { HE_LOG_NFO(_what) }
