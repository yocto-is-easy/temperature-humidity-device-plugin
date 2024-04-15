#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <thread>
#include <chrono>
#include <mutex>

#include "i-plugin-manager.hpp"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <supervisor.hpp>

#define MAX_UDP_PACKET_SIZE 1024

using namespace nlohmann;

typedef enum {
    NOR = 0,
    CYC = 1,
    CMD = 2
} working_mode_t;

typedef struct __attribute__((__packed__)) {
    struct {
        bool busy;
        working_mode_t mode;
        bool calibrated;
    } status;
    float humidity;
    float temperature;
} aht10_data_t;

enum class TemperatureFormat {
    CELSIUS, // received from sensor
    FARANHEIT,
    KELVIN
};

class TemperatureHumidityService {
private:
    TemperatureFormat _temperature_format;
    float _temperature; // contains celcius
    float _humidity; // contains percentage

    std::mutex _mutex;

    float get_temperature_formated() {
        std::lock_guard<std::mutex> lock(_mutex);
        switch (_temperature_format) {
            case TemperatureFormat::CELSIUS:
                return _temperature;
            case TemperatureFormat::FARANHEIT:
                return (_temperature * 9.0f / 5.0f) + 32.0f;
            case TemperatureFormat::KELVIN:
                return _temperature + 273.15f;
            default:
                return _temperature;
        }
    }

public:
    TemperatureHumidityService() : _temperature_format(TemperatureFormat::CELSIUS)
                                  , _temperature(0.0f)
                                  , _humidity(0.0f) {
        std::thread([this]() {
            int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if (sockfd < 0) {
                spdlog::error("Failed to create UDP socket");
                return;
            }

            struct sockaddr_in server_addr{};
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = INADDR_ANY;
            server_addr.sin_port = htons(1234);

            if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
                spdlog::error("Failed to bind UDP socket");
                close(sockfd);
                return;
            }

            while (true) {
                char buffer[MAX_UDP_PACKET_SIZE];
                struct sockaddr_in client_addr{};
                socklen_t client_addr_len = sizeof(client_addr);

                ssize_t num_bytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_addr_len);
                if (num_bytes < 0) {
                    spdlog::error("Failed to receive UDP packet");
                    continue;
                }

                if (num_bytes != sizeof(aht10_data_t)) {
                    spdlog::error("Invalid UDP packet size");
                    continue;
                }

                aht10_data_t* data = (aht10_data_t*)buffer;
                {
                    std::lock_guard<std::mutex> lock(_mutex);
                    _temperature = data->temperature;
                    _humidity = data->humidity;
                }

                spdlog::info("Received temperature: {} and humidity: {}", data->temperature, data->humidity);
            }

            close(sockfd);
        }).detach();
    }

    float get_temperature() {
        return get_temperature_formated();
    }

    float get_humidity() {
        std::lock_guard<std::mutex> lock(_mutex);
        return _humidity;
    }

    void set_temperature_format(TemperatureFormat format) {
        std::lock_guard<std::mutex> lock(_mutex);
        _temperature_format = format;
    }
};
