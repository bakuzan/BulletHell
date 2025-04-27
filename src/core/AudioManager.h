#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SFML/Audio.hpp>

#include <string>
#include <unordered_map>

#include "constants/AudioId.h"

class AudioManager
{
private:
    std::unordered_map<AudioId, sf::SoundBuffer> soundBuffers;
    std::unordered_map<AudioId, sf::Sound> sounds;

    std::unordered_map<AudioId, float> volumeMap;

private:
    void initializeVolumeDefaults();

public:
    AudioManager();
    ~AudioManager();

    void loadSound(AudioId uniqueId, const std::string &filename);

    sf::Sound &getSound(AudioId uniqueId);
    const sf::Sound::Status getSoundStatus(AudioId uniqueId) const;

    void playSound(AudioId uniqueId, bool loop = false);
};

#endif // AUDIOMANAGER_H
