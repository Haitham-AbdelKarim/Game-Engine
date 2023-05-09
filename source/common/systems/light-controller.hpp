#pragma once

#include "../components/mesh-renderer.hpp"
#include "../components/light.hpp"
#include "../material/material.hpp"
#include "../shader/shader.hpp"

#include "../ecs/world.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <string>

#define MAX_LIGHTS 8 

struct Light {
    int type;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    glm::vec3 attenuation;
    glm::vec2 cone_angles;
};

namespace our {

class LightSystem {
public:
  // This should be called every frame to update all entities containing a
  // MovementComponent.
  void update(World *world, float deltaTime) {
    int lightCount = 0;
    Light light_list [MAX_LIGHTS];
    // For each entity in the world
    for (auto entity : world->getEntities()) {
      // Get the movement component if it exists
      LightComponent *light = entity->getComponent<LightComponent>();
      // If the movement component exists
      if (light) {
        light_list[lightCount].color =light->color; 
        light_list[lightCount].attenuation =light->attenuation; 
        light_list[lightCount].cone_angles =light->cone_angles; 
        light_list[lightCount].type =light->type; 
        light_list[lightCount].position = entity->getLocalToWorldMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);; 
        light_list[lightCount].direction = light->direction;
        lightCount++;
      }
    }
    for (auto entity : world->getEntities()) {
      // Get the movement component if it exists
      MeshRendererComponent *meshRenderer =entity->getComponent<MeshRendererComponent>();
      // If the movement component exists
      if (meshRenderer) {
        LitMaterial *litMaterial = dynamic_cast<LitMaterial*>(meshRenderer->material);
        if(litMaterial){
          ShaderProgram *shader = litMaterial->shader;
          for(int i = 0; i < lightCount ; i++){
            std::string uf_loc = "lights[" + std::to_string(i) + "]";
            shader->set((uf_loc)+".type" , light_list[i].type);
            shader->set((uf_loc)+".color" , light_list[i].color);
            shader->set((uf_loc)+".position" , light_list[i].position);
            shader->set((uf_loc)+".direction" , light_list[i].direction);
            shader->set((uf_loc)+".attenuation" , light_list[i].attenuation);
            shader->set((uf_loc)+".cone_angles" , light_list[i].cone_angles);
          }
          shader->set("light_count",lightCount);
          shader->set("sky.top",glm::vec3(0.0f, 0.1f, 0.5f));
          shader->set("sky.horizon",glm::vec3(0.3f, 0.3f, 0.3f));
          shader->set("sky.bottom",glm::vec3(0.1f, 0.1f, 0.1f));
        }
        
      }
    }
  }
};

} // namespace our
