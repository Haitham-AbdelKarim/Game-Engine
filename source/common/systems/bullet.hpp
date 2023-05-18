#pragma once

#include "../components/bullet.hpp"
#include "../ecs/world.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>

namespace our {

class BulletSystem {
  float time = 0;

public:
  void update(World *world, float deltaTime) {
    time += deltaTime;
    // For each entity in the world
    for (auto entity : world->getEntities()) {
      BulletComponent *bullet = entity->getComponent<BulletComponent>();
      // If the movement component exists
      if (bullet) {
        // Change the position and rotation based on the linear & angular
        // velocity and delta time.
        if (time > bullet->shotTime + 1) {
          world->markForRemoval(entity);
        }
      }
    }
    world->deleteMarkedEntities();
  }
};

} // namespace our
