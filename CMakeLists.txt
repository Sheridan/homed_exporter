cmake_minimum_required(VERSION 3.12)
project(homed_exporter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost 1.74 REQUIRED COMPONENTS system)
find_package(PahoMqttCpp REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)
# pkg_check_modules(PahoMqttCpp REQUIRED paho-mqttpp)

option(CMAKE_BUILD_TYPE "Build type" Release)

message("Configuring as " ${CMAKE_BUILD_TYPE})
if (CMAKE_BUILD_TYPE STREQUAL Debug)
  set(CMAKE_VERBOSE_MAKEFILE ON)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
  add_definitions(-DHE_DEBUG)
endif (CMAKE_BUILD_TYPE STREQUAL Debug)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/version.txt" PACKAGE_VERSION)
string(STRIP "${PACKAGE_VERSION}" PACKAGE_VERSION)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h"
  @ONLY
)

set(HE_HEADERS
  src/configuration/ccmdoptions.h
  src/configuration/cconfiguration.h
  src/defines/class_helpers.h
  src/defines/metrics.h
  src/http/chttp.h
  src/http/types.h
  src/log.h
  src/main.h
  src/model/chomed.h
  src/model/device/cdevice.h
  src/model/device/cdevices.h
  src/model/device/types.h
  src/model/metrics/cmetricbulk.h
  src/model/metrics/cmetricname.h
  src/model/metrics/cmetrics.h
  src/model/metrics/cmetricvalue.h
  src/model/metrics/types/cmetric.h
  src/model/metrics/types/cmetricincremental.h
  src/model/metrics/types/cmetricpartial.h
  src/model/metrics/types/cmetricstatic.h
  src/mqtt/cdevicename.h
  src/mqtt/cmqttcallback.h
  src/mqtt/cmqttclient.h
  src/mqtt/cpayload.h
  src/mqtt/ctopic.h
  src/st.h
  src/tools/singletone/csingletone.h
  src/tools/singletone/singletone.h
  src/utils/datetime.h
  src/utils/string.h
  )

set(HE_SOURCES
  src/configuration/ccmdoptions.cpp
  src/configuration/cconfiguration.cpp
  src/http/chttp.cpp
  src/main.cpp
  src/model/chomed.cpp
  src/model/device/cdevice.cpp
  src/model/device/cdevices.cpp
  src/model/device/types.cpp
  src/model/metrics/cmetricbulk.cpp
  src/model/metrics/cmetricname.cpp
  src/model/metrics/cmetrics.cpp
  src/model/metrics/cmetricvalue.cpp
  src/model/metrics/types/cmetric.cpp
  src/model/metrics/types/cmetricincremental.cpp
  src/model/metrics/types/cmetricpartial.cpp
  src/model/metrics/types/cmetricstatic.cpp
  src/mqtt/cdevicename.cpp
  src/mqtt/cmqttcallback.cpp
  src/mqtt/cmqttclient.cpp
  src/mqtt/cpayload.cpp
  src/mqtt/ctopic.cpp
  src/tools/singletone/csingletone.cpp
  src/utils/datetime.cpp
  src/utils/string.cpp
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
set(CPACK_PACKAGE_VERSION "${PACKAGE_VERSION}")
set(CPACK_PACKAGE_CONTACT "Gorlov Maxim <sheridan@babylo-five.ru>")
set(CPACK_PACKAGE_DESCRIPTION "HOMEd Exporter is a tool to export HOMEd Zigbee data metrics.")
set(CPACK_PACKAGE_MAINTAINER "Gorlov Maxim <sheridan@babylon-five.ru>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libpaho-mqttpp3, libboost-system, libjsoncpp1")
set(CPACK_DEBIAN_PACKAGE_USER "homed")
set(CPACK_DEBIAN_PACKAGE_GROUP "homed")
set(CPACK_DEBIAN_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}_${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/deploy/debian/control.extra")
set(CPACK_DEBIAN_PACKAGE_POST_INSTALL "${CMAKE_CURRENT_SOURCE_DIR}/deploy/debian//post_install.sh")
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/deploy/homed_exporter.service
        DESTINATION /usr/share/homed_exporter)

include(CPack)
