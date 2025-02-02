#pragma once

#include <External/json.hpp>
#include <fstream>
#include <stdexcept>
#include <string>

using json = nlohmann::json;

constexpr float millisecondsToSeconds = 1 / 1000.f;

constexpr char* GAMEINFOJSON_CONFIG = "../Data/Config/GameInfo_Config.json";
constexpr char* GAMEINFOJSON_ENEMIES = "../Data/Config/GameInfo_Enemies.json";
constexpr char* GAMEINFOJSON_PLAYER = "../Data/Config/GameInfo_Player.json";
constexpr char* GAMEINFOJSON_PICKUPS = "../Data/Config/GameInfo_PickUps.json";
constexpr char* GAMEINFOJSON_UI = "../Data/Config/GameInfo_UI.json";
constexpr char* GAMEINFOJSON_LEVEL = "../Data/Config/GameInfo_Level.json";
constexpr char* GAMEINFOJSON_AUDIO = "../Data/Config/GameInfo_Audio.json";

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