#pragma once
#include <components/player.hpp>
#include <ecs/entity.hpp>
#include <reactphysics3d/reactphysics3d.h>

namespace our {

class CollisionHandler {
public:
  void playerLanded(Entity *player) {
    player->getComponent<PlayerComponent>()->onTheGround = true;
    reactphysics3d::Vector3 velocity =
        player->getComponent<RigidBodyComponent>()
            ->rigidbody->getLinearVelocity();

    player->getComponent<RigidBodyComponent>()->rigidbody->setLinearVelocity(
        reactphysics3d::Vector3(velocity.x, 0, velocity.z));
  }
};

} // namespace our
