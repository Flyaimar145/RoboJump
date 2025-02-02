#include <Core/AudioManager.h>
#include <External/json.hpp>
#include <Utils/Constants.h>

using json = nlohmann::json;

AudioManager* AudioManager::s_instance{ nullptr };

AudioManager* AudioManager::getInstance()
{
    if (s_instance == nullptr)
    {
        s_instance = new AudioManager();
    }
    return s_instance;
}

AudioManager::~AudioManager()
{
    stopCurrentMusic();
    stopAllSounds();

    for (auto& [type, music] : m_musicsMap)
    {
        delete music;
        music = nullptr;
    }
    m_musicsMap.clear();

    m_soundsMap.clear();
    m_soundBuffersMap.clear();
	m_loadedSoundsPathsMap.clear();
}

bool AudioManager::loadAllAudio() 
{
    json audioConfig = loadJsonFromFile(GAMEINFOJSON_AUDIO);

    const auto& musicConfig = audioConfig["Music"];
    for (auto& [key, value] : musicConfig.items()) 
    {
        MusicType musicType = parseMusicType(key);
        loadMusic(musicType, value.get<std::string>());
    }

    const auto& soundConfig = audioConfig["FX"];
    for (auto& [key, value] : soundConfig.items()) 
    {
        SoundType soundType = parseSoundType(key);
        loadSound(soundType, value.get<std::string>());
    }

    return true;
}

bool AudioManager::loadMusic(MusicType musicType, const std::string& filePath)
{
    sf::Music* music = new sf::Music();
    if (!music->openFromFile(filePath))
    {
		printf("Error: Could not load music from %s\n", filePath.c_str());
        return false;
    }
    m_currentMusic = musicType;
    m_musicsMap[m_currentMusic] = music;
    return true;
}

bool AudioManager::loadSound(SoundType soundType, const std::string& filePath)
{
    if (m_loadedSoundsPathsMap.find(soundType) != m_loadedSoundsPathsMap.end() && m_loadedSoundsPathsMap[soundType] == filePath)
    {
        return true;
    }

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filePath))
    {
		printf("Error: Could not load sound from %s\n", filePath.c_str());
        return false;
    }

    m_soundBuffersMap[soundType] = buffer;
    m_soundsMap[soundType].setBuffer(m_soundBuffersMap[soundType]);
    m_loadedSoundsPathsMap[soundType] = filePath;

    return true;
}

void AudioManager::playMusic(MusicType musicType, bool hasToLoop, float volume)
{
    stopCurrentMusic();
    auto it = m_musicsMap.find(musicType);
    if (it != m_musicsMap.end()) 
    {
        m_currentMusic = musicType;
        it->second->setLoop(hasToLoop);
        it->second->setVolume(volume);
        it->second->play();
    }
    else 
    {
		printf("Error: Music not found");
    }
}

void AudioManager::playSound(SoundType soundType, float volume)
{
    auto it = m_soundsMap.find(soundType);
    if (it != m_soundsMap.end())
    {
        it->second.setVolume(volume);
        it->second.play();
    }
    else
    {
		printf("Error: Sound not found");
    }
}

void AudioManager::stopCurrentMusic()
{
    m_musicsMap[m_currentMusic]->stop();
}

void AudioManager::stopAllSounds()
{
    for (auto& [type, sound] : m_soundsMap)
    {
        sound.stop();
    }
}



