#pragma once

#include "../components/bullet.hpp"
#include "../components/rigid-body.hpp"
#include "../ecs/world.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>

#include <iostream>
#include <reactphysics3d/reactphysics3d.h>

// ReactPhysics3D namespace

namespace our {

// The movement system is responsible for moving every entity which contains a
// MovementComponent. This system is added as a simple example for how use the
// ECS framework to implement logic. For more information, see
// "common/components/movement.hpp"
class RigidBodySystem {
public:
  // This should be called every frame to update all entities containing
  // a MovementComponent.
  void update(World *world, float deltaTime,
              reactphysics3d::PhysicsWorld *phyWorld,
              reactphysics3d::PhysicsCommon *physicsCommon) {

    // For each entity in the world
    for (auto entity : world->getEntities()) {
      // Get the movement component if it exists
      RigidBodyComponent *rigidBody =
          entity->getComponent<RigidBodyComponent>();

      // If the movement component exists
      if (rigidBody) {
        // Change the position and rotation based on the linear & angular
        // velocity and delta time.
        if (!rigidBody->isInitialized) {
          rigidBody->setup(phyWorld, entity, physicsCommon);
          if (entity->name == "bullet") {
            rigidBody->rigidbody->setLinearVelocity(
                convert(entity->getComponent<BulletComponent>()->direction) *
                20);
            rigidBody->rigidbody->enableGravity(false);
            rigidBody->rigidbody->setMass(0.1);
          }
        }
        entity->localTransform.position =
            convert(rigidBody->rigidbody->getTransform().getPosition());
        // entity->localTransform.rotation +=
        //     deltaTime * movement->angularVelocity;
      }
    }
  }
};

} // namespace our
