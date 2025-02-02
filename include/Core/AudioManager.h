#pragma once

#include <Core/AudioTypes.h>
#include <SFML/Audio.hpp>
#include <unordered_map>

class AudioManager
{
    public:

        ~AudioManager();

        static AudioManager* getInstance();

        bool loadAllAudio();

        void playMusic(MusicType type, bool loop = true, float volume = 100.f);
        void playSound(SoundType type, float volume = 100.f);

        void stopCurrentMusic();
        void stopAllSounds();

    private:

        static AudioManager* s_instance;

        std::unordered_map<MusicType, sf::Music*> m_musicsMap;

        std::unordered_map<SoundType, sf::Sound> m_soundsMap;
        std::unordered_map<SoundType, sf::SoundBuffer> m_soundBuffersMap;
        std::unordered_map<SoundType, std::string> m_loadedSoundsPathsMap;
        
        MusicType m_currentMusic = MusicType::MainMenu;

        bool loadMusic(MusicType type, const std::string& filePath);
        bool loadSound(SoundType type, const std::string& filePath);
};
