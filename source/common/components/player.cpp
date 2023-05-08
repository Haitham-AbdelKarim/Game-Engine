
#include "player.hpp"
#include "../deserialize-utils.hpp"
#include "../ecs/entity.hpp"


namespace our {

void PlayerComponent::deserialize(const nlohmann::json &data) {
  if (!data.is_object())
    return;
}
} // namespace our