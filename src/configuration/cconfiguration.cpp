#include "configuration/cconfiguration.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include "st.h"
#include "cconfiguration.h"

namespace he
{
namespace configuration
{

CConfiguration::CConfiguration()
{}

void CConfiguration::load(const std::string &filename)
{
  std::ifstream ifs(filename, std::ifstream::binary);
  if (!ifs.is_open())
  {
    HE_ABORT("Can not open config file " + filename);
  }


  Json::CharReaderBuilder readerBuilder;
  std::string errs;
  if (!Json::parseFromStream(readerBuilder, ifs, &m_json, &errs))
  {
    std::cerr << "Error parsing config file: " << errs << std::endl;
    HE_ABORT("Can not parse config file");
  }
}

bool CConfiguration::labelsExists(const std::string &name)
{
  return listExists("exporter", "labels", name);
}

const Json::Value &CConfiguration::labels(const std::string &name)
{
  return list("exporter", "labels", name);
}

bool CConfiguration::ignoresExists(const std::string &name)
{
  return listExists("exporter", "ignores", name);
}

const Json::Value &CConfiguration::ignores(const std::string &name)
{
  return list("exporter", "ignores", name);
}

bool CConfiguration::listExists(const std::string &containerName, const std::string &listName, const std::string &name)
{
  return m_json[containerName].isMember(listName) && m_json[containerName][listName].isMember(name);
}

const Json::Value &CConfiguration::list(const std::string &containerName, const std::string &listName, const std::string &name)
{
  return m_json[containerName][listName][name];
}

CConfiguration::~CConfiguration()
{}

}
}
