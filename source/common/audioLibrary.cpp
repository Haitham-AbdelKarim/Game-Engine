#pragma once
#include <audioLibrary.hpp>
#include <iostream>

namespace our {
std::map<std::string, sf::SoundBuffer *> AudioLibrary::soundMap;

void AudioLibrary::addSound(std::string path, std::string name) {
  sf::SoundBuffer *newSound = new sf::SoundBuffer;
  newSound->loadFromFile(path);
  AudioLibrary::soundMap.insert({name, newSound});
}

sf::SoundBuffer *AudioLibrary::getSound(std::string name) {
  return AudioLibrary::soundMap[name];
}

} // namespace our
