#ifndef COLORS_CLI_HPP
#define COLORS_CLI_HPP

#include <string>
#include <unordered_map>

namespace CliColors {
enum class Colors_enum { WHITE, RED, GREEN, BLUE, RESET };

inline const std::unordered_map<Colors_enum, std::string> escape_codes = {
    {Colors_enum::RESET, "\e[0m"},
    {Colors_enum::RED, "\e[31m"},
    {Colors_enum::BLUE, "\e[34m"},
    {Colors_enum::GREEN, "\e[42m"},
    {Colors_enum::WHITE, "\e[37m"}};

// the default is WHITE
inline Colors_enum StrToColor(const std::string &s) {
  return s == "blue"    ? Colors_enum::BLUE
         : s == "red"   ? Colors_enum::RED
         : s == "green" ? Colors_enum::GREEN
         : s == "white" ? Colors_enum::WHITE
                        : Colors_enum::WHITE;
}

}; // namespace CliColors

#endif
