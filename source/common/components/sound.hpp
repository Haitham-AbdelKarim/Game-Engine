#pragma once

#include "../ecs/component.hpp"
#include <SFML/Audio.hpp>
#include <string>

namespace our {

class SoundComponent : public Component {
public:
  std::string soundName;
  float volume = 50.0;
  float minDistance = 4.0f;
  float attenuation = 10.0f;
  float pitch = 1.0f;
  bool loop = true;
  bool autoPlay = true;
  sf::Sound sound;

  void setup();
  void play();

  static std::string getID() { return "Sound"; }

  // Reads linearVelocity & angularVelocity from the given json object
  void deserialize(const nlohmann::json &data) override;
};

} // namespace our