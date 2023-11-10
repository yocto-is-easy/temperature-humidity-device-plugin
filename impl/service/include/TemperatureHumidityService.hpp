#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <map>

#include "i-plugin-manager.hpp"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <supervisor.hpp>

using namespace nlohmann;

class TemperatureHumidityService {
private:

public:
    TemperatureHumidityService() {
        spdlog::info("TemperatureHumidityService: constructor");
    }

    ~TemperatureHumidityService() {
        spdlog::info("TemperatureHumidityService: destructor");
    }

    float get_temperature() {
        spdlog::info("TemperatureHumidityService: get_temperature");
        return 22.0f;
    }

    float get_humidity() {
        spdlog::info("TemperatureHumidityService: get_humidity");
        return 51.0f;
    }
};
