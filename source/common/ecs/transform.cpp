#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>
#include <iostream>

using namespace std;

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //TODO: (Req 3) Write this function
        cout<<"toMat4 1"<<endl;
        glm::mat4 scaleingMat = glm::scale(glm::mat4(1.0f), scale);
        cout<<"toMat4 2"<<endl;
        glm::mat4 rotationMat = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        cout<<"toMat4 3"<<endl;
        glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), position);
        cout<<"toMat4 4"<<endl;
        
        return (translationMat * rotationMat *  scaleingMat); 
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}