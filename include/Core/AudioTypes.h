#pragma once

#include <SFML/Audio.hpp>
#include <unordered_map>

enum class MusicType
{
    Game,
    MainMenu
};

enum class SoundType
{
    EntityDeath,
    EntityHurt,
    FrogAttack,
    GameOver,
    PickUpCollected,
    PlayerJump,
    StartGame,
    StompAttack,
    Victory
};

inline MusicType parseMusicType(const std::string& musicType) 
{
    static const std::unordered_map<std::string, MusicType> musicMap = 
    {
        {"Game", MusicType::Game},
        {"MainMenu", MusicType::MainMenu}
    };

    auto it = musicMap.find(musicType);
    if (it != musicMap.end()) 
    {
        return it->second;
    }
    else 
    {
        printf("Error: MusicType not found for %s\n", musicType.c_str());
        return MusicType::MainMenu;
    }
}

inline SoundType parseSoundType(const std::string& soundType) 
{
    static const std::unordered_map<std::string, SoundType> soundMap = 
    {
        {"EntityDeath", SoundType::EntityDeath},
        {"EntityHurt", SoundType::EntityHurt},
        {"FrogAttack", SoundType::FrogAttack},
        {"GameOver", SoundType::GameOver},
        {"PickUpCollected", SoundType::PickUpCollected},
        {"PlayerJump", SoundType::PlayerJump},
        {"StartGame", SoundType::StartGame},
        {"StompAttack", SoundType::StompAttack},
        {"Victory", SoundType::Victory}
    };

    auto it = soundMap.find(soundType);
    if (it != soundMap.end()) 
    {
        return it->second;
    }
    else 
    {
		printf("Error: SoundType not found for %s\n", soundType.c_str());
        return SoundType::StartGame;
    }
}

