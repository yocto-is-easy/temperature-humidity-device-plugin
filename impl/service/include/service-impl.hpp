#pragma once

#include <thread>
#include <string>
#include <chrono>
#include <memory>

#include <nlohmann/json.hpp>

#include "temperature-humidity-device-plugin/i-temperature-humidity-device-plugin.hpp"
#include "i-plugin-manager.hpp"
#include "TemperatureHumidityService.hpp"

#include <supervisor.hpp>

using namespace std;

std::shared_ptr<TemperatureHumidityService> m_temperatureHumidityService = nullptr;

INIT_MIDF_SERVER(temperature_humidity_device_plugin);

MIDF_IMPL_FUNC(midf::JsonAsRet, temperature_humidity_device_plugin, get_temperature, midf::JsonAsParam) (midf::JsonAsParam param) {
    float temperature = m_temperatureHumidityService->get_temperature();

    midf::JsonAsRet ret;
    ret.get()["return"] = temperature;

    return ret;
}

MIDF_IMPL_FUNC(midf::JsonAsRet, temperature_humidity_device_plugin, get_humidity, midf::JsonAsParam) (midf::JsonAsParam param) {
    float humidity = m_temperatureHumidityService->get_humidity();

    midf::JsonAsRet ret;
    ret.get()["return"] = humidity;

    return ret;
}

MIDF_IMPL_FUNC(midf::JsonAsRet, temperature_humidity_device_plugin, sum, midf::JsonAsParam) (midf::JsonAsParam param) {
    int x, y;
    midf::JsonAsRet ret;

    try{
        std::string x_str = param.get()["x"];
        std::string y_str = param.get()["y"];

        x = std::stoi(x_str);
        y = std::stoi(y_str);
    } catch(const std::exception& e) {
        spdlog::error("sum::get_temperature_humidity: {}", e.what());

        ret.get()["return"] = "error";
        return ret;
    }

    ret.get()["return"] = x + y;
    return ret;
}

void start_temperature_humidity_device_plugin_service(std::shared_ptr<TemperatureHumidityService> temperatureHumidityService) {
    m_temperatureHumidityService = temperatureHumidityService;

    plugin_manager::init_plugin("temperature_humidity_device_plugin");

    plugin_manager::register_function(
        "temperature_humidity_device_plugin",
        "get_temperature",
        AS_CALL_BACK(temperature_humidity_device_plugin, get_temperature, midf::JsonAsRet, midf::JsonAsParam),
        midf::JsonAsVar({
            {"description", "returns temperature in celcius"},
            {"return", {
                {"type", "float"},
                {"description", "temperature in celcius"}
            }},
            {"arguments", json::array()}
    }));

    plugin_manager::register_function(
        "temperature_humidity_device_plugin",
        "get_humidity",
        AS_CALL_BACK(temperature_humidity_device_plugin, get_humidity, midf::JsonAsRet, midf::JsonAsParam),
        midf::JsonAsVar({
            {"description", "returns humidity in %"},
            {"return", {
                {"type", "float"},
                {"description", "humidity in relative %"}
            }},
            {"arguments", json::array()}
    }));

    plugin_manager::register_function(
        "temperature_humidity_device_plugin",
        "sum",
        AS_CALL_BACK(temperature_humidity_device_plugin, sum, midf::JsonAsRet, midf::JsonAsParam),
        midf::JsonAsVar({
            {"description", "returns sum of two numbers"},
            {"return", {
                {"type", "int"},
                {"description", "sum of two numbers"}
            }},
            {"arguments", json::array({
                {
                    {"name", "x"},
                    {"type", "int"},
                    {"description", "first number"}
                },
                {
                    {"name", "y"},
                    {"type", "int"},
                    {"description", "second number"}
                }
            })}
    }));

    START_MIDF_SERVER(temperature_humidity_device_plugin);
}
