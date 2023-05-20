#include "sound.hpp"
#include "../deserialize-utils.hpp"
#include "ecs/entity.hpp"
#include <audioLibrary.hpp>
namespace our {

void SoundComponent::deserialize(const nlohmann::json &data) {
  if (!data.is_object())
    return;
  soundName = data.value("sound", soundName);
  attenuation = data.value("attenuation", attenuation);
  volume = data.value("volume", volume);
  loop = data.value("loop", loop);
  minDistance = data.value("minDistance", minDistance);
  pitch = data.value("pitch", pitch);
  autoPlay = data.value("autoPlay", autoPlay);
  setup();
  if (autoPlay)
    play();
}

void SoundComponent::setup() {
  sound.setPitch(pitch);
  sound.setVolume(volume);
  auto soundBuffer = AudioLibrary::getSound(soundName);
  sound.setBuffer(*soundBuffer);
  sound.setMinDistance(minDistance);
  sound.setAttenuation(attenuation);
  glm::vec3 position = glm::vec3(getOwner()->getLocalToWorldMatrix() *
                                 glm::vec4(0.0, 0.0, 0.0, 1.0));
  sound.setPosition(position.x, position.y, position.z);
  sound.setLoop(loop);
}
void SoundComponent::play() { sound.play(); }

} // namespace our