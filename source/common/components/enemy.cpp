
#include "enemy.hpp"
#include "../deserialize-utils.hpp"

namespace our {

void EnemyComponent::deserialize(const nlohmann::json &data) {
  if (!data.is_object())
    return;
}
} // namespace our