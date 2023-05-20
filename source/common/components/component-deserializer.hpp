#pragma once

#include "../ecs/entity.hpp"
#include "bullet.hpp"
#include "camera.hpp"
#include "enemy.hpp"
#include "follower.hpp"
#include "free-camera-controller.hpp"
#include "light.hpp"
#include "mesh-renderer.hpp"
#include "movement.hpp"
#include "player.hpp"
#include "rigid-body.hpp"
#include "sound.hpp"
#include "spawner.hpp"


namespace our {

// Given a json object, this function picks and creates a component in the given
// entity based on the "type" specified in the json object which is later
// deserialized from the rest of the json object
inline void deserializeComponent(const nlohmann::json &data, Entity *entity) {
  std::string type = data.value("type", "");
  Component *component = nullptr;
  // TODO: (Req 8) Add an option to deserialize a "MeshRendererComponent" to the
  // following if-else statement
  if (type == CameraComponent::getID()) {
    component = entity->addComponent<CameraComponent>();
  } else if (type == FreeCameraControllerComponent::getID()) {
    component = entity->addComponent<FreeCameraControllerComponent>();
  } else if (type == MovementComponent::getID()) {
    component = entity->addComponent<MovementComponent>();
  } else if (type == MeshRendererComponent::getID()) {
    component = entity->addComponent<MeshRendererComponent>();
  } else if (type == RigidBodyComponent::getID()) {
    component = entity->addComponent<RigidBodyComponent>();
  } else if (type == PlayerComponent::getID()) {
    component = entity->addComponent<PlayerComponent>();
  } else if (type == LightComponent::getID()) {
    component = entity->addComponent<LightComponent>();
  } else if (type == SpawnerComponent::getID()) {
    component = entity->addComponent<SpawnerComponent>();
  } else if (type == FollowerComponent::getID()) {
    component = entity->addComponent<FollowerComponent>();
  } else if (type == BulletComponent::getID()) {
    component = entity->addComponent<BulletComponent>();
  } else if (type == EnemyComponent::getID()) {
    component = entity->addComponent<EnemyComponent>();
  } else if (type == SoundComponent::getID()) {
    component = entity->addComponent<SoundComponent>();
  }
  if (component)
    component->deserialize(data);
}

} // namespace our