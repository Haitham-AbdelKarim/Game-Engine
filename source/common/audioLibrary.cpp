#pragma once
#include <audioLibrary.hpp>
#include <iostream>

namespace our {
std::map<std::string, sf::SoundBuffer *> AudioLibrary::soundMap;
std::map<std::string, sf::Music *> AudioLibrary::musicMap;

void AudioLibrary::addSound(std::string path, std::string name) {
  sf::SoundBuffer *newSound = new sf::SoundBuffer;
  newSound->loadFromFile(path);
  AudioLibrary::soundMap.insert({name, newSound});
}

sf::SoundBuffer *AudioLibrary::getSound(std::string name) {
  return AudioLibrary::soundMap[name];
}
void AudioLibrary::addMusic(std::string path, std::string name) {
  sf::Music *newMusic = new sf::Music();
  newMusic->openFromFile(path);
  AudioLibrary::musicMap.insert({name, newMusic});
}
sf::Music *AudioLibrary::getMusic(std::string name) {
  return AudioLibrary::musicMap[name];
}
void AudioLibrary::clear() {
  for (auto i = AudioLibrary::soundMap.begin();
       i != AudioLibrary::soundMap.end(); ++i) {
    delete i->second;
  }
  for (auto i = AudioLibrary::musicMap.begin();
       i != AudioLibrary::musicMap.end(); ++i) {
    delete i->second;
  }
}

} // namespace our
