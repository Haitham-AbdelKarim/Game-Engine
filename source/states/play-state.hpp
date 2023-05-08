#pragma once

#include <application.hpp>

#include <asset-loader.hpp>
#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <systems/player-controller.hpp>
#include <systems/rigid-body.hpp>
#include <utils.hpp>

// This state shows how to use the ECS framework and deserialization.
class Playstate : public our::State {

  our::World world;
  our::ForwardRenderer renderer;
  our::FreeCameraControllerSystem cameraController;
  our::MovementSystem movementSystem;
  our::RigidBodySystem rigidBodySystem;
  our::PlayerControllerSystem playerControllerSystem;

  reactphysics3d::PhysicsCommon physicsCommon;
  const double timeStep = 1.0f / 60.0f;

  // Create a physics world
  reactphysics3d::PhysicsWorld *worldPhysics;

  void onInitialize() override {
    // First of all, we get the scene configuration from the app config
    auto &config = getApp()->getConfig()["scene"];
    // If we have assets in the scene config, we deserialize them
    if (config.contains("assets")) {
      our::deserializeAllAssets(config["assets"]);
    }
    // If we have a world in the scene config, we use it to populate our world
    if (config.contains("world")) {
      world.deserialize(config["world"]);
    }
    // We initialize the camera controller system since it needs a pointer to
    // the app
    cameraController.enter(getApp());
    playerControllerSystem.enter(getApp());
    // Then we initialize the renderer
    auto size = getApp()->getFrameBufferSize();
    renderer.initialize(size, config["renderer"]);
    worldPhysics = physicsCommon.createPhysicsWorld();
  }

  void onDraw(double deltaTime) override {
    // Here, we just run a bunch of systems to control the world logic
    movementSystem.update(&world, (float)deltaTime);
    // And finally we use the renderer system to draw the scene
    renderer.render(&world, worldPhysics, &physicsCommon);
    cameraController.update(&world, (float)deltaTime);
    playerControllerSystem.update(&world, (float)deltaTime);

    rigidBodySystem.update(&world, (float)deltaTime);

    worldPhysics->update(timeStep);
    // Get a reference to the keyboard object
    auto &keyboard = getApp()->getKeyboard();

    if (keyboard.justPressed(GLFW_KEY_ESCAPE)) {
      // If the escape  key is pressed in this frame, go to the play state
      getApp()->changeState("menu");
    }
  }

  void onDestroy() override {
    // Don't forget to destroy the renderer
    renderer.destroy();
    // On exit, we call exit for the camera controller system to make sure that
    // the mouse is unlocked
    cameraController.exit();
    playerControllerSystem.exit();
    // Clear the world
    world.clear();
    // and we delete all the loaded assets to free memory on the RAM and the
    // VRAM
    our::clearAllAssets();
  }
};