#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>

namespace our {

class AudioLibrary {

public:
  static std::map<std::string, sf::SoundBuffer *> soundMap;

  static void addSound(std::string path, std::string name);
  static sf::SoundBuffer* getSound(std::string name);
};

} // namespace our