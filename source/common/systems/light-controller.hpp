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
  std::vector<Light> getlights(World *world, float deltaTime) {
    int lightCount = 0;
    std::vector<Light> light_list;
    // For each entity in the world
    Light lightData;
    for (auto entity : world->getEntities()) {
      // Get the movement component if it exists
      LightComponent *light = entity->getComponent<LightComponent>();
      // If the movement component exists
      if (light) {
        lightData.color = light->color;
        lightData.attenuation = light->attenuation;
        lightData.cone_angles = light->cone_angles;
        lightData.type = light->lightType;
        lightData.position =
            entity->getLocalToWorldMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        ;
        lightData.direction = light->direction;
        lightCount++;
        light_list.push_back(lightData);
      }
    }
    return (light_list);
  }
};

} // namespace our
