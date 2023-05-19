#pragma once

#include "../ecs/component.hpp"
#include <ecs/entity.hpp>
#include <string>
#include <utils.hpp>

namespace our {

class SpawnerComponent : public Component {
public:
  Entity *entity;
  float lastSpawn = 0;
  static std::string getID() { return "Spawner"; }
  Entity *spawn(World *world);
  // Reads linearVelocity & angularVelocity from the given json object
  void deserialize(const nlohmann::json &data) override;
};

} // namespace our