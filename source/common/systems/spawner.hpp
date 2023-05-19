#pragma once

#include "../components/spawner.hpp"
#include "../ecs/world.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <random>
#include <vector>

namespace our {

class SpawnerSystem {
  Application *app;          // The application in which the state runs
  bool mouse_locked = false; // Is the mouse locked
  int round = 0;
  int roundEnemyCount = 0;
  int spawnedEnemyCount = 0;
  int currentEnemyCount = 0;
  float time = 0;
  float spawnRate = 5;
  float roundFinishTime = 0;
  float timeBetweenRounds = 15;
  bool roundFinished = false;
  int firstRoundstartTime = 15;

public:
  // When a state enters, it should call this function and give it the
  // pointer to the application
  void enter(Application *app) { this->app = app; }

  void decrementCurrentEnemyCount() { currentEnemyCount--; }

  // This should be called every frame to update all entities containing a
  // FreeCameraControllerComponent
  void update(World *world, float deltaTime, int &currentRound) {
    // First of all, we search for an entity containing both a CameraComponent
    // and a FreeCameraControllerComponent As soon as we find one, we break
    time += deltaTime;
    if (time > firstRoundstartTime) {
      if (round != currentRound) {
        round = currentRound;
        std::cout << '\n' << round;
        if (round == 1) {
          roundEnemyCount = 4;
        }
        float temp = (float)(roundEnemyCount * 1.5);
        roundEnemyCount = (int)std::ceil(temp);
      }
    }

    SpawnerComponent *spawner = nullptr;
    std::vector<SpawnerComponent *> enemysSpawners;
    for (auto entity : world->getEntities()) {
      if (auto spawner = entity->getComponent<SpawnerComponent>(); spawner) {
        if (entity->name == "enemy Spawner") {
          enemysSpawners.push_back(spawner);
        }
      }
    }
    if (currentEnemyCount < 24) {
      if (spawnedEnemyCount < roundEnemyCount) {
        if (enemysSpawners.size()) {
          int index = (std::rand() % (enemysSpawners.size()));
          if (time > (enemysSpawners[index]->lastSpawn + spawnRate)) {
            enemysSpawners[index]->spawn(world);
            enemysSpawners[index]->lastSpawn = time;
            spawnedEnemyCount++;
            currentEnemyCount++;
          }
        }
      }
    }
    if (currentEnemyCount == 0 && spawnedEnemyCount != 0 && !roundFinished) {
      roundFinishTime = time;
      roundFinished = true;
    }
    if ((time > roundFinishTime + timeBetweenRounds) && roundFinished) {
      currentRound++;
      spawnedEnemyCount = 0;
      roundFinished = false;
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
