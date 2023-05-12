#pragma once

#include "../components/light.hpp"
#include "../components/mesh-renderer.hpp"
#include "../material/material.hpp"
#include "../shader/shader.hpp"

#include "../ecs/world.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>
#include <string>
#include <utils.hpp>

#define MAX_LIGHTS 8

namespace our {

class LightSystem {
public:
  // This should be called every frame to update all entities containing a
  // MovementComponent.
  Light *getlights(World *world, float deltaTime) {
    int lightCount = 0;
    Light *light_list = new Light[MAX_LIGHTS];
    // For each entity in the world
    for (auto entity : world->getEntities()) {
      // Get the movement component if it exists
      LightComponent *light = entity->getComponent<LightComponent>();
      // If the movement component exists
      if (light) {
        light_list[lightCount].color = light->color;
        light_list[lightCount].attenuation = light->attenuation;
        light_list[lightCount].cone_angles = light->cone_angles;
        light_list[lightCount].type = light->lightType;
        light_list[lightCount].position =
            entity->getLocalToWorldMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        ;
        light_list[lightCount].direction = light->direction;
        lightCount++;
      }
    }
    return (light_list);
  }
};

} // namespace our
