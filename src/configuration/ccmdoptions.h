#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <iostream>
#include <sstream>
#include <variant>



namespace he
{
namespace configuration
{

class CCmdLine
{
public:
  CCmdLine();

  template <typename T>
  void addOption(const std::string& shortOpt, const std::string& longOpt,
                const std::string& description, const T& defaultValue)
  {
    SOptionDefinition optionDef { defaultValue, defaultValue, description, !std::is_same<T, bool>::value };
    m_options[longOpt] = optionDef;
    m_short2Long[shortOpt] = longOpt;
  }

  bool parse(int argc, char* argv[]);

  template <typename T>
  T option(const std::string& name) const
  {
    if (m_options.find("--" + name) == m_options.end())
    {
      throw std::runtime_error("Option not found: " + name);
    }
    return std::get<T>(m_options.at("--" + name).value);
  }

private:
  typedef std::variant<bool, int, std::string> TOptionValue;
  struct SOptionDefinition
  {
      TOptionValue defaultValue;
      TOptionValue value;
      std::string description;
      bool takesParameter;
  };

  std::unordered_map<std::string, SOptionDefinition> m_options;
  std::unordered_map<std::string, std::string> m_short2Long;

  void printHelp() const;
};



}
}
