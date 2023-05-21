#pragma once

#include "../ecs/component.hpp"
#include <string>
#include <utils.hpp>

namespace our {

// This component denotes that the MovementSystem will move the owning entity by
// a certain linear and angular velocity. This component is added as a simple
// example for how use the ECS framework to implement logic. For more
// information, see "common/systems/movement.hpp" For a more complex example of
// how to use the ECS framework, see "free-camera-controller.hpp"
class PlayerComponent : public Component {
public:
  // The senstivity paramter defined sensitive the camera rotation & fov is to
  // the mouse moves and wheel scrolling
  float rotationSensitivity =
      0.01f; // The angle change per pixel of mouse movement
  float fovSensitivity =
      0.3f; // The fov angle change per unit of mouse wheel scrolling
  glm::vec3 positionSensitivity = {
      3.0f, 3.0f,
      3.0f}; // The unity per second of camera movement if WASD is pressed
  float speedupFactor =
      5.0f; // A multiplier for the positionSensitivity if "Left Shift" is held.
  int maxHealth = 4;
  int currentHealth = maxHealth;
  float damage = 100;
  float lastHit = 0;
  // The ID of this component type is "Player"
  bool onTheGround = true;
  static std::string getID() { return "Player"; }

  // Reads linearVelocity & angularVelocity from the given json object
  void deserialize(const nlohmann::json &data) override;
};

} // namespace our