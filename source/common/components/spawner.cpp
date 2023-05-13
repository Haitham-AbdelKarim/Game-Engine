
#include "spawner.hpp"
#include "../deserialize-utils.hpp"
#include "../ecs/entity.hpp"
#include "../ecs/world.hpp"
#include "follower.hpp"
#include "mesh-renderer.hpp"
#include "rigid-body.hpp"


namespace our {

void SpawnerComponent::deserialize(const nlohmann::json &data) {
  if (!data.is_object())
    return;
  entity = World::addspawner();
  entity->parent = nullptr;
  entity->deserialize(data["entity"]);
}
void SpawnerComponent::spawn(World *world) {
  Entity *newEntity = world->add();
  newEntity->localTransform = entity->localTransform;
  newEntity->name = entity->name;
  if (auto meshRenderer = entity->getComponent<MeshRendererComponent>();
      meshRenderer) {
    auto newMeshRenderer = newEntity->addComponent<MeshRendererComponent>();
    newMeshRenderer->copy(meshRenderer);
  }
  if (auto rigidBody = entity->getComponent<RigidBodyComponent>(); rigidBody) {
    auto newRigidBody = newEntity->addComponent<RigidBodyComponent>();
    newRigidBody->copy(rigidBody);
  }
  if (auto follower = entity->getComponent<FollowerComponent>(); follower) {
    auto newFollower = newEntity->addComponent<FollowerComponent>();
    newFollower->copy(follower);
  }
}
} // namespace our