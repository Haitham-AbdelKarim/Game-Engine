#pragma once
#include <components/bullet.hpp>
#include <components/enemy.hpp>
#include <components/player.hpp>
#include <components/rigid-body.hpp>
#include <ecs/entity.hpp>
#include <iostream>
#include <reactphysics3d/reactphysics3d.h>
#include <systems/spawner.hpp>

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
  void bulletHit(Entity *enemy, Entity *bullet) {
    auto enemyComponent = enemy->getComponent<EnemyComponent>();
    auto bulletComponent = bullet->getComponent<BulletComponent>();
    if (enemyComponent && bulletComponent) {
      enemyComponent->health -= bulletComponent->damage;
      bulletComponent->damage = 0;
    }
  }
};

} // namespace our
