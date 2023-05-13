#pragma once

#include "../components/spawner.hpp"
#include "../ecs/world.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>

namespace our {

class SpawnerSystem {
  Application *app;          // The application in which the state runs
  bool mouse_locked = false; // Is the mouse locked

public:
  // When a state enters, it should call this function and give it the pointer
  // to the application
  void enter(Application *app) { this->app = app; }

  // This should be called every frame to update all entities containing a
  // FreeCameraControllerComponent
  void update(World *world, float deltaTime) {
    // First of all, we search for an entity containing both a CameraComponent
    // and a FreeCameraControllerComponent As soon as we find one, we break
    SpawnerComponent *spawner = nullptr;
    for (auto entity : world->getEntities()) {
      if (auto spawner = entity->getComponent<SpawnerComponent>(); spawner) {
        if (app->getKeyboard().isPressed(GLFW_KEY_R))
          spawner->spawn(world);
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
