#pragma once
#include "audioLibrary.hpp"
#include <components/bullet.hpp>
#include <components/enemy.hpp>
#include <components/player.hpp>
#include <components/rigid-body.hpp>
#include <ecs/entity.hpp>
#include <iostream>
#include <reactphysics3d/reactphysics3d.h>
#include <systems/forward-renderer.hpp>
#include <systems/spawner.hpp>

namespace our {

class CollisionHandler {
  float lastHitTime = 0;
  SpawnerSystem *spawnerSystem = nullptr;
  sf::Sound collectSound;

public:
  void setspawner(SpawnerSystem *spawner) { spawnerSystem = spawner; }
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
      auto playerComponent = player->getComponent<PlayerComponent>();
      playerComponent->currentHealth--;
      playerComponent->lastHit = time;
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
  void collectedMushroom(Entity *player, Entity *mushroom) {
    player->getComponent<PlayerComponent>()->damage += 5;
    spawnerSystem->removeMushroom(mushroom);
    collectSound.setBuffer(*AudioLibrary::getSound("collect"));
    collectSound.play();
  }
  void playMusic(std::string musicName) {
    if (AudioLibrary::getMusic(musicName)->getStatus() ==
        sf::SoundSource::Status::Stopped) {
      AudioLibrary::getMusic(musicName)->stop();
    }
    AudioLibrary::getMusic(musicName)->play();
  }
};

} // namespace our
