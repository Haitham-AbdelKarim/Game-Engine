
#include "bullet.hpp"
#include "../deserialize-utils.hpp"

namespace our {

void BulletComponent::deserialize(const nlohmann::json &data) {
  if (!data.is_object())
    return;
}
} // namespace our