#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <thread>

int main() {
  auto now =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  std::cout << std::put_time(std::localtime(&now), "%H:%M:%S") << '\n';
  std::this_thread::sleep_for(std::chrono::seconds(1));
}
