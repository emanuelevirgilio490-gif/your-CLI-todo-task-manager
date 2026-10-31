/*
This file.hpp contains all of the constant
for the file and exit codes used in this projects

I used constant instead of only numbers for readability
*/

#ifndef DEFINES_HPP
#define DEFINES_HPP

#include "cli-colors.hpp"
#include <cstdint>
#include <cstdlib>
#include <string>

// the file where the tasks are stored
// read only because cannot be used in std::fstream to open the file
const std::string FILE_PATH_READ_ONLY = "~/.tasks.json";

// file path info
const std::string FILE_NAME = ".tasks";
const std::string FILE_EXTENSION = ".json";
const std::string FILE_FULL_NAME = ".tasks.json";
const std::string USER_HOME = std::getenv("HOME");
const std::string FILE_FULL_PATH = USER_HOME + "/" + FILE_FULL_NAME;

// exit codes
const uint8_t ENUM_NOT_DEFINED = 1;
const int8_t FILE_DOES_NOT_EXIST = -1;
const uint8_t TYPE_NOT_VALID = 2;
const int8_t FILE_NOT_OPEN = -2;

// configs ERRORS
const int8_t CONFIG_NOT_OPENED = -10;
const int8_t NO_PS1 = -11;
const std::string PS1_ERROR = "__ERROR__";
const int8_t PS1_TEXT_NOT_STR = -12;
const int8_t PS1_IS_NIL = -13;
// config info
const std::string CONFIG_NAME = ".prd-cli-config";
const std::string CONFIG_EXTENSION = ".lua";
const std::string CONFIF_FILE_FULL_NAME = CONFIG_NAME + CONFIG_EXTENSION;
const std::string CONFIG_FULL_PATH = USER_HOME + "/" + CONFIF_FILE_FULL_NAME;
#endif
