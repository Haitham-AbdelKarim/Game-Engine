#pragma once

#include "../ecs/component.hpp"
#include <ecs/entity.hpp>
#include <string>
#include <utils.hpp>

namespace our {

class BulletComponent : public Component {
public:
  glm::vec3 direction = glm::vec3(0, 0, -1);
  float shotTime = 0;
  static std::string getID() { return "Bullet"; }
  void deserialize(const nlohmann::json &data) override;
};

} // namespace our