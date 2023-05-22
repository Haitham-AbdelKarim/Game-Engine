#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>

namespace our {

class AudioLibrary {

public:
  static std::map<std::string, sf::SoundBuffer *> soundMap;
  static std::map<std::string, sf::Music *> AudioLibrary::musicMap;

  static void addSound(std::string path, std::string name);
  static sf::SoundBuffer *getSound(std::string name);
  static void addMusic(std::string path, std::string name);
  static sf::Music *getMusic(std::string name);
  static void clear();
  ~AudioLibrary() { clear(); }
};

} // namespace our