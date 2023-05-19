#pragma once

#include "../ecs/component.hpp"
#include <ecs/entity.hpp>
#include <string>
#include <utils.hpp>

namespace our {

class EnemyComponent : public Component {
public:
  float health = 60;
  static std::string getID() { return "Enemy"; }
  void deserialize(const nlohmann::json &data) override;
};

} // namespace our