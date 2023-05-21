#pragma once

#include "../components/follower.hpp"
#include "../components/sound.hpp"
#include "../components/spawner.hpp"
#include "../ecs/world.hpp"


#include "../application.hpp"

#include "../audioLibrary.hpp"
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
  int round;
  int roundEnemyCount;
  int spawnedEnemyCount;
  int currentEnemyCount;
  float time;
  float spawnRate;
  float roundFinishTime;
  float timeBetweenRounds;
  bool roundFinished;
  int firstRoundstartTime;
  int screamingEnemysCount;
  int maxScreamingEnemysCount;
  float lastMushroomSpawnedTime;
  float timeBetweenMushroomSpawnes;
  Entity *spawnedMushrooms[6];
  bool MushroomsIndeciesToRemove[6];

public:
  // When a state enters, it should call this function and give it the
  // pointer to the application
  void enter(Application *app) {
    this->app = app;
    round = 0;
    roundEnemyCount = 0;
    roundEnemyCount = 0;
    spawnedEnemyCount = 0;
    currentEnemyCount = 0;
    time = 0;
    spawnRate = 5;
    roundFinishTime = 0;
    timeBetweenRounds = 15;
    roundFinished = false;
    firstRoundstartTime = 15;
    screamingEnemysCount = 0;
    maxScreamingEnemysCount = 2;
    lastMushroomSpawnedTime = 0;
    timeBetweenMushroomSpawnes = 10;
    spawnedMushrooms[0] = nullptr;
    spawnedMushrooms[1] = nullptr;
    spawnedMushrooms[2] = nullptr;
    spawnedMushrooms[3] = nullptr;
    spawnedMushrooms[4] = nullptr;
    spawnedMushrooms[5] = nullptr;
    MushroomsIndeciesToRemove[0] = false;
    MushroomsIndeciesToRemove[1] = false;
    MushroomsIndeciesToRemove[2] = false;
    MushroomsIndeciesToRemove[3] = false;
    MushroomsIndeciesToRemove[4] = false;
    MushroomsIndeciesToRemove[5] = false;
  }

  void decrementCurrentEnemyCount() { currentEnemyCount--; }
  void removeMushroom(Entity *mushroom) {
    for (int i = 0; i < 6; i++) {
      if (mushroom == spawnedMushrooms[i]) {
        MushroomsIndeciesToRemove[i] = true;
      }
    }
  }

  // This should be called every frame to update all entities containing a
  // FreeCameraControllerComponent
  void update(World *world, float deltaTime, int &currentRound) {
    // First of all, we search for an entity containing both a CameraComponent
    // and a FreeCameraControllerComponent As soon as we find one, we break
    time += deltaTime;
    if (time > firstRoundstartTime) {
      if (round != currentRound) {
        round = currentRound;
        if (round == 1) {
          roundEnemyCount = 4;
          if (AudioLibrary::getMusic("roundStart")->getStatus() ==
              sf::SoundSource::Status::Stopped) {
            AudioLibrary::getMusic("roundStart")->stop();
          }
          AudioLibrary::getMusic("roundStart")->play();
        }
        float temp = (float)(roundEnemyCount * 1.5);
        roundEnemyCount = (int)std::ceil(temp);
      }
    }

    SpawnerComponent *spawner = nullptr;
    std::vector<SpawnerComponent *> enemysSpawners;
    std::vector<SpawnerComponent *> mushroomSpawners;

    for (auto entity : world->getEntities()) {
      if (auto spawner = entity->getComponent<SpawnerComponent>(); spawner) {
        if (entity->name == "enemy Spawner") {
          enemysSpawners.push_back(spawner);
        }
        if (entity->name == "mushroom Spawner") {
          mushroomSpawners.push_back(spawner);
        }
      }
    }

    if (time > lastMushroomSpawnedTime + timeBetweenMushroomSpawnes) {
      int index = (std::rand() % (mushroomSpawners.size()));
      if (!(spawnedMushrooms[index])) {
        spawnedMushrooms[index] = mushroomSpawners[index]->spawn(world);
        lastMushroomSpawnedTime = time;
      }
    }

    for (int i = 0; i < 6; i++) {
      if (MushroomsIndeciesToRemove[i]) {
        world->markForRemoval(spawnedMushrooms[i]);
        spawnedMushrooms[i] = nullptr;
        MushroomsIndeciesToRemove[i] = false;
      }
    }
    world->deleteMarkedEntities();

    if (currentEnemyCount < 24) {
      if (spawnedEnemyCount < roundEnemyCount) {
        if (enemysSpawners.size()) {
          int index = (std::rand() % (enemysSpawners.size()));
          if (time > (enemysSpawners[index]->lastSpawn + spawnRate)) {
            Entity *enemy = enemysSpawners[index]->spawn(world);
            enemysSpawners[index]->lastSpawn = time;
            spawnedEnemyCount++;
            currentEnemyCount++;
            auto enemyComponent = enemy->getComponent<EnemyComponent>();
            enemyComponent->health = enemyComponent->health * round;
            if (round > 2) {
              int index = (std::rand() % (enemysSpawners.size()));
              if (index == 0) {
                enemy->getComponent<FollowerComponent>()->speed =
                    enemy->getComponent<FollowerComponent>()->speed * 2;
              }
            }
          }
        }
      }
    }
    screamingEnemysCount = 0;
    for (auto entity : world->getEntities()) {
      if (auto soundComponent = entity->getComponent<SoundComponent>();
          soundComponent) {
        if (entity->name == "enemy") {
          if (soundComponent->sound.getStatus() !=
              sf::SoundSource::Status::Playing) {
            soundComponent->play();
            screamingEnemysCount++;
          } else {
            screamingEnemysCount++;
          }
        }
        if (screamingEnemysCount == maxScreamingEnemysCount)
          break;
      }
    }
    if (currentEnemyCount == 0 && spawnedEnemyCount != 0 && !roundFinished) {
      roundFinishTime = time;
      roundFinished = true;
      if (AudioLibrary::getMusic("roundChange")->getStatus() ==
          sf::SoundSource::Status::Stopped) {
        AudioLibrary::getMusic("roundChange")->stop();
      }
      AudioLibrary::getMusic("roundChange")->play();
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
