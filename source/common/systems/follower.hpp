#pragma once

#include "../components/follower.hpp"
#include "../components/rigid-body.hpp"
#include "../ecs/world.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
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
              direction = glm::normalize(direction) * follower->speed;
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
