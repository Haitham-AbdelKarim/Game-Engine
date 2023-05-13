#pragma once

#include "../ecs/component.hpp"
#include <string>

namespace our {

class FollowerComponent : public Component {
public:
  std::string targetName;
  float speed = 1;
  static std::string getID() { return "Follower"; }
  void copy(FollowerComponent *copied) { targetName = copied->targetName; }
  // Reads linearVelocity & angularVelocity from the given json object
  void deserialize(const nlohmann::json &data) override;
};

} // namespace our