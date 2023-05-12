#pragma once
#include <ecs/entity.hpp>
#include <reactphysics3d/reactphysics3d.h>
#include <string>
#include <systems/collision-handler.hpp>

namespace our {
class PhysicsEventsListener : public reactphysics3d::EventListener {

  enum Contacts {
    none = -1,
    Player_Ground = 0,
  };

public:
  CollisionHandler collisionHandler;
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
    } else {
      return none;
    }
  }
};
} // namespace our