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

MIDF_IMPL_FUNC(midf::JsonAsRet, temperature_humidity_device_plugin, set_temperature_format, midf::JsonAsParam) (midf::JsonAsParam param) {
    std::string temperature_format = param.get()["temperature_format"];

    midf::JsonAsRet ret;
    ret.get()["return"] = 0;

    if (temperature_format == "Celsius") {
        m_temperatureHumidityService->set_temperature_format(TemperatureFormat::CELSIUS);
    } else if (temperature_format == "Faranheit") {
        m_temperatureHumidityService->set_temperature_format(TemperatureFormat::FARANHEIT);
    } else if (temperature_format == "Kelvin") {
        m_temperatureHumidityService->set_temperature_format(TemperatureFormat::KELVIN);
    } else {
        ret.get()["return"] = -1;
        return ret;
    }

    return ret;
}

void start_temperature_humidity_device_plugin_service(std::shared_ptr<TemperatureHumidityService> temperatureHumidityService) {
    m_temperatureHumidityService = temperatureHumidityService;

    std::string plugin_name = "temperature-humidity-device-plugin";

    plugin_manager::init_plugin(plugin_name);

    plugin_manager::register_function(
        plugin_name,
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
        plugin_name,
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
        plugin_name,
        "set_temperature_format",
        AS_CALL_BACK(temperature_humidity_device_plugin, set_temperature_format, midf::JsonAsRet, midf::JsonAsParam),
        midf::JsonAsVar({
            {"description", "sets the format of temperature to be returned"},
            {"return", {
                {"type", "int"},
                {"description", "0 for success, -1 for failure"}
            }},
            {"arguments", json::array({
                {
                    {"name", "temperature_format"},
                    {"type", "string"},
                    {"description", "accepts 'Celsius', 'Faranheit', 'Kelvin'"}
                }
            })}
    }));

    START_MIDF_SERVER(temperature_humidity_device_plugin);
}
