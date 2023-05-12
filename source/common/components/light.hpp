#pragma once

#include "../ecs/component.hpp"
#include <string>
#include <utils.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace our {

class LightComponent : public Component {
public:

    //DIRECTIONAL = 0, POINT = 1,  SPOT = 2
    glm::vec3 color  = glm::vec3(0,0,0);
    glm::vec3 attenuation  = glm::vec3(0,0,0);
    glm::vec2 cone_angles  = glm::vec2(0.5f*glm::quarter_pi<float>(),0.5f*glm::quarter_pi<float>());
    glm::vec3 direction = glm::vec3(0,0,-1);
    int lightType = 0;

    // The ID of this component type is "Light"
    static std::string getID() { return "Light"; }

    void deserialize(const nlohmann::json &data) override;
};

} // namespace our