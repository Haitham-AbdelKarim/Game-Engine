#pragma once

#include <glm/glm.hpp>
#include <reactphysics3d/reactphysics3d.h>

static glm::vec3 convert(reactphysics3d::Vector3 data) {
  return glm::vec3(data.x, data.y, data.z);
}
static reactphysics3d::Vector3 convert(glm::vec3 data) {
  return reactphysics3d::Vector3((reactphysics3d::decimal)data.x,
                                 (reactphysics3d::decimal)data.y,
                                 (reactphysics3d::decimal)data.z);
}
static reactphysics3d::Matrix3x3 convert(glm::mat3 data) {
  reactphysics3d::Vector3 row0 =
      convert(glm::normalize(glm::vec3(data[0][0], data[1][0], data[2][0])));
  reactphysics3d::Vector3 row1 =
      convert(glm::normalize(glm::vec3(data[0][1], data[1][1], data[2][1])));
  reactphysics3d::Vector3 row2 =
      convert(glm::normalize(glm::vec3(data[0][2], data[1][2], data[2][2])));
  return reactphysics3d::Matrix3x3(row0.x, row0.y, row0.z, row1.x, row1.y,
                                   row1.z, row2.x, row2.y, row2.z);
}

static reactphysics3d::Transform convert(glm::mat4 transform) {
  glm::vec3 position = glm::vec3(transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  glm::mat3 rotation = glm::mat3(transform);
  reactphysics3d::Transform trans =
      reactphysics3d::Transform(convert(position), convert(rotation));
  return trans;
}
