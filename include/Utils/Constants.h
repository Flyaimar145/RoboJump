#pragma once
#include <External/json.hpp>
#include <string>
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

constexpr float millisecondsToSeconds = 1 / 1000.f;

constexpr char* GAMEINFOJSON_CONFIG = "../Data/Config/GameInfo_Config.json";
constexpr char* GAMEINFOJSON_ENEMIES = "../Data/CONFIG/GameInfo_Enemies.json";
constexpr char* GAMEINFOJSON_PLAYER = "../Data/CONFIG/GameInfo_Player.json";

inline json loadJsonFromFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    json data;
    file >> data;
    return data;
}