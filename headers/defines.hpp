#ifndef DEFINES_HPP
#define DEFINES_HPP

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
const uint8_t FILE_DOES_NOT_EXIST = -1;

#endif
