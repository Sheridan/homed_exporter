cmake_minimum_required(VERSION 3.12)
project(homed_exporter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PahoMqttCpp REQUIRED)
find_package(Boost 1.74 REQUIRED COMPONENTS system)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

# find_package(PkgConfig REQUIRED)
# pkg_check_modules(PROMETHEUS_CPP REQUIRED prometheus-cpp)
# pkg_check_modules(PAHO_MQTT_CPP REQUIRED PahoMqttCpp)
# pkg_check_modules(JSONCPP REQUIRED jsoncpp)
# pkg_check_modules(LOGGERLIB REQUIRED loggerlib)

option(CMAKE_BUILD_TYPE "Build type" Release)

message("Configuring as " ${CMAKE_BUILD_TYPE})
if (CMAKE_BUILD_TYPE STREQUAL Debug)
  set(CMAKE_VERBOSE_MAKEFILE ON)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
  add_definitions(-DHE_DEBUG)
endif (CMAKE_BUILD_TYPE STREQUAL Debug)

set(HE_HEADERS
  src/configuration/ccmdoptions.h
  src/configuration/cconfiguration.h
  src/defines/metrics.h
  src/http/chttp.h
  src/http/types.h
  src/main.h
  src/model/chomed.h
  src/model/metrics/cmetricstatic.h
  src/model/metrics/cmetricbulk.h
  src/model/metrics/cmetricincremental.h
  src/model/metrics/cmetricpartial.h
  src/mqtt/cmqttcallback.h
  src/mqtt/cmqttclient.h
  src/mqtt/cpayload.h
  src/st.h
  src/log.h
  src/tools/singletone/csingletone.h
  src/tools/singletone/singletone.h
  src/mqtt/cdevicename.h
  src/model/device/cdevices.h
  src/model/device/cdevice.h
  src/utils/datetime.h
  src/model/metrics/cmetricvalue.h
  src/model/device/types.h
  src/utils/string.h
  src/mqtt/ctopic.h
  src/defines/class_helpers.h
  src/model/metrics/cmetrics.h
  src/model/metrics/cmetric.h
  src/model/metrics/cmetricname.h
  )

set(HE_SOURCES
  src/configuration/ccmdoptions.cpp
  src/configuration/cconfiguration.cpp
  src/http/chttp.cpp
  src/main.cpp
  src/model/chomed.cpp
  src/model/metrics/cmetricstatic.cpp
  src/model/metrics/cmetricbulk.cpp
  src/model/metrics/cmetricincremental.cpp
  src/model/metrics/cmetricpartial.cpp
  src/mqtt/cmqttcallback.cpp
  src/mqtt/cmqttclient.cpp
  src/mqtt/cpayload.cpp
  src/tools/singletone/csingletone.cpp
  src/mqtt/cdevicename.cpp
  src/model/device/cdevices.cpp
  src/model/device/cdevice.cpp
  src/utils/datetime.cpp
  src/model/metrics/cmetricvalue.cpp
  src/model/device/types.cpp
  src/utils/string.cpp
  src/mqtt/ctopic.cpp
  src/model/metrics/cmetrics.cpp
  src/model/metrics/cmetric.cpp
  src/model/metrics/cmetricname.cpp
  )

include_directories(src)

add_executable(homed_exporter
    ${HE_HEADERS}
    ${HE_SOURCES}
)
target_link_libraries(homed_exporter
    PahoMqttCpp::paho-mqttpp3
    jsoncpp
    ${Boost_LIBRARIES}
  )

  install(TARGETS homed_exporter DESTINATION /usr/bin)

set(CPACK_PACKAGE_NAME "homed_exporter")
set(CPACK_PACKAGE_VERSION "0.8.0")
set(CPACK_PACKAGE_CONTACT "Gorlov Maxim <sheridan@babylo-five.ru>")
set(CPACK_PACKAGE_DESCRIPTION "HomeD Exporter is a tool to export HOMEd Zigbee data metrics.")
set(CPACK_PACKAGE_MAINTAINER "Gorlov Maxim <sheridan@babylo-five.ru>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libpaho-mqttpp3, libboost-system, libjsoncpp1")

include(CPack)
