#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

int main(void) {
  std::cout << "opening json...\n";
  nlohmann::json json_data;
  json_data["big-foo"] = {{"foo1", 1}, {"foo2", 2}};

  std::ofstream file("file.json");
  file << json_data.dump(4);
  return 0;
}
