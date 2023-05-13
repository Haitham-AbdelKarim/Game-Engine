#pragma once

#include "../ecs/component.hpp"
#include <ecs/entity.hpp>
#include <string>
#include <utils.hpp>

namespace our {

// This component denotes that the MovementSystem will move the owning entity by
// a certain linear and angular velocity. This component is added as a simple
// example for how use the ECS framework to implement logic. For more
// information, see "common/systems/movement.hpp" For a more complex example of
// how to use the ECS framework, see "free-camera-controller.hpp"
class RigidBodyComponent : public Component {
public:
  reactphysics3d::RigidBody *rigidbody;
  bool isInitialized = false;
  std::string rigidBodyType = "dynamic";
  bool collide = false;
  std::string colliderShape = "box";
  glm::vec3 colliderScale = glm::vec3(1, 1, 1);
  // The ID of this component type is "Movement"
  static std::string getID() { return "Rigid Body"; }
  // initialize
  void setup(reactphysics3d::PhysicsWorld *world, Entity *entity,
             reactphysics3d::PhysicsCommon *physicsCommon);

  // Reads linearVelocity & angularVelocity from the given json object
  void deserialize(const nlohmann::json &data) override;

  RigidBodyComponent() {}
  void copy(RigidBodyComponent *copied) {
    rigidBodyType = copied->rigidBodyType;
    collide = copied->collide;
    colliderShape = copied->colliderShape;
    colliderScale = copied->colliderScale;
  }
};

} // namespace our