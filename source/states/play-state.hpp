#pragma once

#include <application.hpp>

#include "audioLibrary.hpp"
#include <algorithm>
#include <asset-loader.hpp>
#include <ecs/world.hpp>
#include <imgui_impl/imgui_impl_opengl3.h>
#include <systems/bullet.hpp>
#include <systems/enemy.hpp>
#include <systems/follower.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/light-controller.hpp>
#include <systems/movement.hpp>
#include <systems/physics-events-listener.hpp>
#include <systems/player-controller.hpp>
#include <systems/rigid-body.hpp>
#include <systems/spawner.hpp>
#include <utils.hpp>

// This state shows how to use the ECS framework and deserialization.
class Playstate : public our::State {

  our::World world;
  our::ForwardRenderer renderer;
  our::FreeCameraControllerSystem cameraController;
  our::MovementSystem movementSystem;
  our::RigidBodySystem rigidBodySystem;
  our::PlayerControllerSystem playerControllerSystem;
  our::LightSystem lightSystem;
  our::PhysicsEventsListener physicsEventsListener;
  our::SpawnerSystem spawnerSystem;
  our::FollowerSystem followerSystem;
  our::BulletSystem bulletSystem;
  our::EnemySystem enemySystem;

  reactphysics3d::PhysicsCommon physicsCommon;
  const double timeStep = 1.0f / 30.0f;
  int round = 1;
  glm::ivec2 size;

  // Create a physics world
  reactphysics3d::PhysicsWorld *worldPhysics;

  void onInitialize() override {
    round = 1;
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
    spawnerSystem.enter(getApp());
    // Then we initialize the renderer
    size = getApp()->getFrameBufferSize();
    renderer.initialize(size, config["renderer"]);
    worldPhysics = physicsCommon.createPhysicsWorld();
    worldPhysics->setEventListener(&physicsEventsListener);
    enemySystem.setSpawner(&spawnerSystem);
    physicsEventsListener.setspawner(&spawnerSystem);
  }
  void onImmediateGui() override {
    ImGui::GetFont()->Scale = 3.0f;
    ImGui::PushFont(ImGui::GetFont());
    ImDrawList *drawList = ImGui::GetBackgroundDrawList();
    std::string roundString = "Round " + std::to_string(round);
    drawList->AddText(ImVec2(25, 25), ImColor(1.0f, 0.0f, 0.0f),
                      roundString.c_str());
    drawList->AddCircle(ImVec2((float)(size.x / 2.0), (float)(size.y / 2.0)),
                        10.0, IM_COL32(255, 255, 255, 255), 100, 1.0f);

    ImGui::PopFont();
  }
  void onDraw(double deltaTime) override {
    // Here, we just run a bunch of systems to control the world logic
    movementSystem.update(&world, (float)deltaTime);
    std::vector<our::Light> light_list =
        lightSystem.getlights(&world, (float)deltaTime);
    rigidBodySystem.update(&world, (float)deltaTime, worldPhysics,
                           &physicsCommon);
    cameraController.update(&world, (float)deltaTime);
    playerControllerSystem.update(&world, (float)deltaTime);
    spawnerSystem.update(&world, (float)deltaTime, round);
    followerSystem.update(&world, (float)deltaTime, worldPhysics,
                          &physicsCommon);
    physicsEventsListener.update((float)deltaTime);
    bulletSystem.update(&world, (float)deltaTime);
    enemySystem.update(&world, (float)deltaTime);

    // And finally we use the renderer system to draw the scene
    renderer.render(&world, light_list, (float)deltaTime);
    worldPhysics->update((reactphysics3d::decimal)timeStep);
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
    spawnerSystem.exit();
    // Clear the world
    world.clear();
    our::AudioLibrary::clear();
    // and we delete all the loaded assets to free memory on the RAM and the
    // VRAM
    our::clearAllAssets();
  }
};