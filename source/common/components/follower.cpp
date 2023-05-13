
#include "follower.hpp"
#include "../deserialize-utils.hpp"

namespace our {

void FollowerComponent::deserialize(const nlohmann::json &data) {
  if (!data.is_object())
    return;
  targetName = data.value("target Name", targetName);
  speed = data.value("speed", speed);
}
} // namespace our