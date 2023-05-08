
#include "light.hpp"
#include "../deserialize-utils.hpp"
#include "../ecs/entity.hpp"


namespace our {

void LightComponent::deserialize(const nlohmann::json &data) {
  if (!data.is_object()) return;
  type = data.value("type", type);
  color = data.value("color", color);
  attenuation = data.value("attenuation", attenuation);
  cone_angles = data.value("cone_angles", cone_angles);
}
} // namespace our