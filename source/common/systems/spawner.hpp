#pragma once

#include "../components/spawner.hpp"
#include "../ecs/world.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>
#include <random>
#include <vector>

namespace our {

class SpawnerSystem {
  Application *app;          // The application in which the state runs
  bool mouse_locked = false; // Is the mouse locked
  int round = 0;
  int roundEnemyCount = 5;
  int spawnedEnemyCount = 0;
  int currentEnemyCount = 0;

public:
  // When a state enters, it should call this function and give it the pointer
  // to the application
  void enter(Application *app) { this->app = app; }

  // This should be called every frame to update all entities containing a
  // FreeCameraControllerComponent
  void update(World *world, float deltaTime, int &currentRound) {
    // First of all, we search for an entity containing both a CameraComponent
    // and a FreeCameraControllerComponent As soon as we find one, we break
    if (round != currentRound) {
      round = currentRound;
      float temp = (float)(roundEnemyCount * 1.2);
      roundEnemyCount = (int)std::ceil(temp);
    }
    SpawnerComponent *spawner = nullptr;
    std::vector<SpawnerComponent *> enemysSpawners;
    for (auto entity : world->getEntities()) {
      if (auto spawner = entity->getComponent<SpawnerComponent>(); spawner) {
        // if (app->getKeyboard().isPressed(GLFW_KEY_R))
        enemysSpawners.push_back(spawner);
      }
    }
    if (currentEnemyCount < 24) {
      if (spawnedEnemyCount < roundEnemyCount) {
        int index = (std::rand() % (enemysSpawners.size()));
        enemysSpawners[index]->spawn(world);
        spawnedEnemyCount++;
        currentEnemyCount++;
      }
    }
  }
  void exit() {
    if (mouse_locked) {
      mouse_locked = false;
      app->getMouse().unlockMouse(app->getWindow());
    }
  }
};
} // namespace our
