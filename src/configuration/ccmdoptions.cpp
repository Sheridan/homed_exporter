#include "configuration/ccmdoptions.h"
#include <stdexcept>
#include <algorithm>
#include "st.h"

namespace he
{
namespace configuration
{

CCmdLine::CCmdLine()
{
  addOption<bool>("-h", "--help", "Show this help message", false);
}

bool CCmdLine::parse(int argc, char* argv[])
{
  bool parsingSuccess = true;
  std::string currentOption;

  for (int i = 1; i < argc; ++i)
  {
    std::string arg = argv[i];
    if (arg.starts_with("--"))
    {
      currentOption = arg;
    }
    else if (arg.starts_with("-"))
    {
      if (m_short2Long.find(arg) != m_short2Long.end())
      {
        currentOption = m_short2Long[arg];
      }
      else
      {
        HE_LOG_ERR("Unsupported option: " << arg);
        printHelp();
        return false;
      }
    }
    else
    {
        if (currentOption.empty() || m_options.find(currentOption) == m_options.end())
        {
          HE_LOG_ERR("Unexpected parameter: " << arg);
          printHelp();
          return false;
        }
        m_options[currentOption].value = arg;
        currentOption.clear();
    }

    if (arg == "-h" || arg == "--help")
    {
      printHelp();
      exit(0);
    }

    if (!currentOption.empty() && m_options.find(currentOption) != m_options.end())
    {
      if (!m_options[currentOption].takesParameter)
      {
        m_options[currentOption].value = true;
        currentOption.clear();
      }
    }
  }

  // Check for missing parameters in options that require them
  for (const auto& [key, def] : m_options)
  {
      if (std::holds_alternative<std::string>(def.defaultValue) &&
          std::get<std::string>(def.value).empty() &&
          !std::get<std::string>(def.defaultValue).empty())
      {
        m_options[key].value = def.defaultValue;
      }
      else if ( std::holds_alternative<int>(def.defaultValue) &&
                std::get<int>(def.value) == 0 &&
                std::get<int>(def.defaultValue) != 0)
      {
        m_options[key].value = def.defaultValue;
      }
      else if ( std::holds_alternative<bool>(def.defaultValue) &&
                std::get<bool>(def.value) == false &&
                std::get<bool>(def.defaultValue) != false)
      {
        m_options[key].value = def.defaultValue;
      }
      else if (def.takesParameter && currentOption == key)
      {
        HE_LOG_ERR("Missing value for parameter: " << key);
        printHelp();
        return false;
      }
  }

  return parsingSuccess;
}

void CCmdLine::printHelp() const
{
  std::cout << "Usage:" << std::endl;
  for (const auto& [longOpt, definition] : m_options)
  {
    for (const auto& [shortOpt, linkedLongOpt] : m_short2Long)
    {
      if (linkedLongOpt == longOpt)
      {
        std::cout << "  " << shortOpt << ", " << longOpt
                  << "\t" << definition.description << std::endl;
      }
    }
  }
}



}
}
