#include "AudioManager.h"

AudioManager::AudioManager()
{
    initializeVolumeDefaults();
}

AudioManager::~AudioManager()
{
    // Destructor
}

// Publics

void AudioManager::loadSound(AudioId uniqueId, const std::string &filename)
{
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename))
    {
        throw std::runtime_error("Failed to load sound: " + filename);
    }

    soundBuffers[uniqueId] = buffer;

    sf::Sound sound;
    sound.setBuffer(soundBuffers[uniqueId]);
    sound.setVolume(volumeMap[uniqueId]);

    sounds[uniqueId] = sound;
}

sf::Sound &AudioManager::getSound(AudioId uniqueId)
{
    return sounds.at(uniqueId);
}

const sf::Sound::Status AudioManager::getSoundStatus(AudioId uniqueId) const
{
    sf::Sound sound = sounds.at(uniqueId);
    return sound.getStatus();
}

void AudioManager::playSound(AudioId uniqueId, bool loop)
{
    if (sounds.find(uniqueId) != sounds.end())
    {
        sounds[uniqueId].play();
        sounds[uniqueId].setLoop(loop);
    }
}

// Privates

void AudioManager::initializeVolumeDefaults()
{
    volumeMap[AudioId::AMBIENT] = 50.0f;

    volumeMap[AudioId::PROJECTILE_BASIC] = 75.0f;
    volumeMap[AudioId::PROJECTILE_LAZER] = 75.0f;
    volumeMap[AudioId::PROJECTILE_MISSILE] = 75.0f;
    volumeMap[AudioId::PROJECTILE_MISSILE_DEBRIS] = 75.0f;
    volumeMap[AudioId::PROJECTILE_ALIEN_BASIC] = 75.0f;

    volumeMap[AudioId::UPGRADEBOX_HEALTH] = 100.0f;
    volumeMap[AudioId::UPGRADEBOX_WEAPON] = 100.0f;

    volumeMap[AudioId::DEATH_PLAYER] = 100.0f;
}
