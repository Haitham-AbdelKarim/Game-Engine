#pragma once

#include "../components/movement.hpp"
#include "../components/light.hpp"

#include "../ecs/world.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>

namespace our {

class MovementSystem {
public:
  // This should be called every frame to update all entities containing a
  // MovementComponent.
  void update(World *world, float deltaTime) {
    // For each entity in the world
    for (auto entity : world->getEntities()) {
      // Get the movement component if it exists
      LightComponent *light = entity->getComponent<LightComponent>();
      // If the movement component exists
      if (light) {
        // TODO:: Calculate Position and Direction
        entity->getLocalToWorldMatrix() 
      }
    }
  }
};

} // namespace our
