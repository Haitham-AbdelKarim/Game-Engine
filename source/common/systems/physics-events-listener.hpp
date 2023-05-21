#pragma once
#include <ecs/entity.hpp>
#include <iostream>
#include <reactphysics3d/reactphysics3d.h>
#include <systems/collision-handler.hpp>

namespace our {
class PhysicsEventsListener : public reactphysics3d::EventListener {

  enum Contacts {
    none = -1,
    Player_Ground = 0,
    Player_Enemy = 1,
    Bullet_Enemy = 2,
    Player_Mushroom = 3,
    Bullet_Bear = 4,
    Bullet_TeadyBear = 5,
    Bullet_HodaBear = 6
  };
  float time;

public:
  CollisionHandler collisionHandler;
  void setspawner(SpawnerSystem *spawner) {
    collisionHandler.setspawner(spawner);
  }
  void update(float delta) { time += delta; }
  virtual void
  onContact(const CollisionCallback ::CallbackData &callbackData) override {
    // For each contact pair
    for (reactphysics3d::uint p = 0; p < callbackData.getNbContactPairs();
         p++) {
      // Get the contact pair
      CollisionCallback ::ContactPair contactPair =
          callbackData.getContactPair(p);
      // For each contact point of the contact pair
      Entity *entity1 = (Entity *)contactPair.getBody1()->getUserData();
      Entity *entity2 = (Entity *)contactPair.getBody2()->getUserData();

      std::string name1 = entity1->name;
      std::string name2 = entity2->name;
      if (getContactId(name1, name2) == Player_Ground) {
        if (name1 == "player") {
          collisionHandler.playerLanded(entity1);
        } else {
          collisionHandler.playerLanded(entity2);
        }
      } else if (getContactId(name1, name2) == Player_Enemy) {
        if (name1 == "player") {
          collisionHandler.EnemyAttacked(entity1, time);
        } else {
          collisionHandler.EnemyAttacked(entity2, time);
        }
      } else if (getContactId(name1, name2) == Bullet_Enemy) {
        if (name1 == "enemy") {
          collisionHandler.bulletHit(entity1, entity2);
        } else {
          collisionHandler.bulletHit(entity2, entity1);
        }
      } else if (getContactId(name1, name2) == Player_Mushroom) {
        if (name1 == "player") {
          collisionHandler.collectedMushroom(entity1, entity2);
        } else {
          collisionHandler.collectedMushroom(entity2, entity1);
        }
      } else if (getContactId(name1, name2) == Bullet_Bear) {
        collisionHandler.playMusic("Romy");
      } else if (getContactId(name1, name2) == Bullet_TeadyBear) {
        collisionHandler.playMusic("AMystery");
      } else if (getContactId(name1, name2) == Bullet_HodaBear) {
        collisionHandler.playMusic("lol");
      }
    }
  }

  Contacts getContactId(std::string name1, std::string name2) {
    if ((name1 == "player" && name2 == "ground") ||
        (name2 == "player" && name1 == "ground")) {
      return Player_Ground;
    } else if ((name1 == "player" && name2 == "obstacle") ||
               (name2 == "player" && name1 == "obstacle")) {
      return Player_Ground;
    } else if ((name1 == "player" && name2 == "enemy") ||
               (name2 == "player" && name1 == "enemy")) {
      return Player_Enemy;
    } else if ((name1 == "bullet" && name2 == "enemy") ||
               (name2 == "bullet" && name1 == "enemy")) {
      return Bullet_Enemy;
    } else if ((name1 == "player" && name2 == "mushroom") ||
               (name2 == "player" && name1 == "mushroom")) {
      return Player_Mushroom;
    } else if ((name1 == "bullet" && name2 == "bear") ||
               (name2 == "bullet" && name1 == "bear")) {
      return Bullet_Bear;
    } else if ((name1 == "bullet" && name2 == "teadyBear") ||
               (name2 == "bullet" && name1 == "teadyBear")) {
      return Bullet_TeadyBear;
    } else if ((name1 == "bullet" && name2 == "hodaBear") ||
               (name2 == "bullet" && name1 == "hodaBear")) {
      return Bullet_HodaBear;
    } else {
      return none;
    }
  }
};
} // namespace our