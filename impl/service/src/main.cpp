#include <string>
#include <vector>
#include <iostream>
#include <thread>

#include "service-impl.hpp"

int main() {
    std::shared_ptr<TemperatureHumidityService> temperatureHumidityService = std::make_shared<TemperatureHumidityService>();
    start_temperature_humidity_device_plugin_service(temperatureHumidityService);
}
