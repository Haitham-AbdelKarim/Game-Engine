#pragma once

#include "../components/enemy.hpp"
#include "../components/sound.hpp"
#include "../ecs/world.hpp"

#include <SFML/Audio.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>
#include <systems/spawner.hpp>

namespace our {

class EnemySystem {
  SpawnerSystem *spawnerSystem;

public:
  void setSpawner(SpawnerSystem *spawner) { spawnerSystem = spawner; }
  void update(World *world, float deltaTime) {
    // For each entity in the world
    for (auto entity : world->getEntities()) {
      EnemyComponent *enemy = entity->getComponent<EnemyComponent>();

      if (enemy) {
        auto position = glm::vec3(entity->getLocalToWorldMatrix() *
                                  glm::vec4(0.0, 0.0, 0.0, 1.0));
        auto soundComponent = entity->getComponent<SoundComponent>();
        if (soundComponent) {
          soundComponent->sound.setPosition(position.x, position.y, position.z);
        }

        // Change the position and rotation based on the linear & angular
        // velocity and delta time.
        if (enemy->health <= 0) {
          world->markForRemoval(entity);
          spawnerSystem->decrementCurrentEnemyCount();
        }
      }
    }
    world->deleteMarkedEntities();
  }
};

} // namespace our
