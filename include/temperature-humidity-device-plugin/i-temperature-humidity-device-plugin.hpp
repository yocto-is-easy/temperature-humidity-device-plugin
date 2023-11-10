#pragma once

#include "i-plugin-manager.hpp"

#include <supervisor.hpp>

MIDF_DECL_PORT(temperature_humidity_device_plugin, 26001);

MIDF_DECL_FUNC(midf::JsonAsRet, temperature_humidity_device_plugin, get_temperature, midf::JsonAsParam);
MIDF_DECL_FUNC(midf::JsonAsRet, temperature_humidity_device_plugin, get_humidity, midf::JsonAsParam);

MIDF_DECL_FUNC(midf::JsonAsRet, temperature_humidity_device_plugin, sum, midf::JsonAsParam);
