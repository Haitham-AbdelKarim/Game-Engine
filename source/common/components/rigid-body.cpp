
#include "rigid-body.hpp"
#include "../deserialize-utils.hpp"
#include "../ecs/entity.hpp"

namespace our {
// initialize
void RigidBodyComponent::setup(reactphysics3d::PhysicsWorld *world,
                               Entity *entity,
                               reactphysics3d::PhysicsCommon *physicsCommon) {
  rigidbody = world->createRigidBody(convert(entity->getLocalToWorldMatrix()));
  if (rigidBodyType == "static") {
    rigidbody->setType(rp3d::BodyType::STATIC);
  } else if (rigidBodyType == "kinematic") {
    rigidbody->setType(rp3d::BodyType::KINEMATIC);
  }
  if (collide) {
    if (colliderShape == "box") {
      reactphysics3d::CollisionShape *newColliderShape =
          physicsCommon->createBoxShape(convert(colliderScale));
      rigidbody->addCollider(newColliderShape, rp3d::Transform::identity());
    }
  }
  isInitialized = true;
  rigidbody->setUserData(entity);
  physicsWorld = world;
}
// Reads linearVelocity & angularVelocity from the given json object
void RigidBodyComponent::deserialize(const nlohmann::json &data) {
  if (!data.is_object())
    return;
  rigidBodyType = data.value("Rigid Body Type", rigidBodyType);
  collide = data.value("collide", collide);
  colliderShape = data.value("collider Shape", colliderShape);
  colliderScale = data.value("collider Scale", colliderScale);
}
} // namespace our