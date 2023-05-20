#pragma once

#include "../components/follower.hpp"
#include "../components/rigid-body.hpp"
#include "../ecs/world.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>

#include <reactphysics3d/reactphysics3d.h>

namespace our {

class FollowerSystem {
public:
  void update(World *world, float deltaTime,
              reactphysics3d::PhysicsWorld *phyWorld,
              reactphysics3d::PhysicsCommon *physicsCommon) {

    // For each entity in the world

    for (auto entity : world->getEntities()) {
      if (auto follower = entity->getComponent<FollowerComponent>(); follower) {
        for (auto entityToFollow : world->getEntities()) {
          if (entityToFollow->name == follower->targetName) {
            glm::vec3 direction =
                glm::vec3(entityToFollow->getLocalToWorldMatrix() *
                              glm::vec4(0, 0, 0, 1) -
                          follower->getOwner()->getLocalToWorldMatrix() *
                              glm::vec4(0, 0, 0, 1));
            if (auto rigidBody = entity->getComponent<RigidBodyComponent>();
                rigidBody) {
              direction = glm::normalize(direction);
              auto dot = glm::dot(direction, glm::vec3(0.0f, 0.0f, 1.0f));
              auto temp = glm::radians(glm::vec2(180, 0)).x;
              float yAngle = 0;
              if (direction.x >= 0 && direction.z >= 0) {
                yAngle = glm::acos(glm::abs(dot));
              } else if (direction.x < 0 && direction.z > 0) {
                yAngle = temp * 2 - glm::acos(glm::abs(dot));
              } else if (direction.x < 0 && direction.z < 0) {
                yAngle = temp + glm::acos(glm::abs(dot));
              } else {
                yAngle = temp - glm::acos(glm::abs(dot));
              }
              entity->localTransform.rotation =
                  glm::vec3(entity->localTransform.rotation.x, yAngle,
                            entity->localTransform.rotation.z);
              direction *= follower->speed;
              if (rigidBody->isInitialized) {
                direction.y = rigidBody->rigidbody->getLinearVelocity().y;
                rigidBody->rigidbody->setLinearVelocity(convert(direction));
              }
            }
          }
        }
      }
    }
  }
};

} // namespace our
