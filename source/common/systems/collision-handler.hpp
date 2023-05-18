#pragma once
#include <components/player.hpp>
#include <ecs/entity.hpp>
#include <iostream>
#include <reactphysics3d/reactphysics3d.h>

namespace our {

class CollisionHandler {
  float lastHitTime = 0;

public:
  void playerLanded(Entity *player) {
    player->getComponent<PlayerComponent>()->onTheGround = true;
    reactphysics3d::Vector3 velocity =
        player->getComponent<RigidBodyComponent>()
            ->rigidbody->getLinearVelocity();

    player->getComponent<RigidBodyComponent>()->rigidbody->setLinearVelocity(
        reactphysics3d::Vector3(velocity.x, 0, velocity.z));
  }
  void EnemyAttacked(Entity *player, float time) {
    if (time > lastHitTime + 1) {
      lastHitTime = time;
      std::cout << "\nDie, b*tch";
      player->getComponent<PlayerComponent>()->currentHealth--;
    }
  }
};

} // namespace our
