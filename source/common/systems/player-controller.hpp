#pragma once

#include "../components/bullet.hpp"
#include "../components/camera.hpp"
#include "../components/player.hpp"
#include "../components/rigid-body.hpp"
#include "../components/spawner.hpp"
#include "../ecs/world.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>
#include <utils.hpp>

namespace our {

// The free camera controller system is responsible for moving every entity
// which contains a FreeCameraControllerComponent. This system is added as a
// slightly complex example for how use the ECS framework to implement logic.
// For more information, see "common/components/free-camera-controller.hpp"
class PlayerControllerSystem {
  Application *app;          // The application in which the state runs
  bool mouse_locked = false; // Is the mouse locked
  float time;
  float lastShot = 0;
  float fireRate = 0.5f;

public:
  // When a state enters, it should call this function and give it the pointer
  // to the application
  void enter(Application *app) { this->app = app; }

  // This should be called every frame to update all entities containing a
  // FreeCameraControllerComponent
  void update(World *world, float deltaTime) {
    // First of all, we search for an entity containing both a CameraComponent
    // and a FreeCameraControllerComponent As soon as we find one, we break
    time += deltaTime;
    CameraComponent *camera = nullptr;
    RigidBodyComponent *rigidbody = nullptr;
    PlayerComponent *player = nullptr;
    for (auto entity : world->getEntities()) {
      camera = entity->getComponent<CameraComponent>();
      player = entity->getComponent<PlayerComponent>();
      rigidbody = entity->getComponent<RigidBodyComponent>();
      if (camera && player)
        break;
    }
    // If there is no entity with both a CameraComponent and a
    // FreeCameraControllerComponent, we can do nothing so we return
    if (!(camera && player))
      return;
    // Get the entity that we found via getOwner of camera (we could use
    // controller->getOwner())
    Entity *entity = camera->getOwner();

    // If the left mouse button is pressed, we lock and hide the mouse. This
    // common in First Person Games.
    if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && !mouse_locked) {
      app->getMouse().lockMouse(app->getWindow());
      mouse_locked = true;
    }
    // If the left ALT button is pressed, we unlock and unhide the mouse.
    else if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_ALT) && mouse_locked) {
      app->getMouse().unlockMouse(app->getWindow());
      mouse_locked = false;
    }

    // We get a reference to the entity's position and rotation
    glm::vec3 position = entity->localTransform.position;
    glm::vec3 &rotation = entity->localTransform.rotation;

    // while the mouse is locked, we get the change in
    // the mouse location and use it to update the camera rotation
    if (mouse_locked) {
      glm::vec2 delta = app->getMouse().getMouseDelta();
      rotation.x -= delta.y * player->rotationSensitivity; // The y-axis
                                                           // controls the pitch
      rotation.y -= delta.x * player->rotationSensitivity; // The x-axis
                                                           // controls the yaw
    }

    // We prevent the pitch from exceeding a certain angle from
    // the XZ plane to prevent gimbal locks
    if (rotation.x < -glm::half_pi<float>() * 0.99f)
      rotation.x = -glm::half_pi<float>() * 0.99f;
    if (rotation.x > glm::half_pi<float>() * 0.99f)
      rotation.x = glm::half_pi<float>() * 0.99f;
    // This is not necessary, but whenever the rotation goes
    // outside the 0 to 2*PI range, we wrap it back inside. This
    // could prevent floating point error if the player rotates in
    // single direction for an extremely long time.
    rotation.y = glm::wrapAngle(rotation.y);

    // We update the camera fov based on the mouse wheel scrolling
    // amount
    float fov = camera->fovY +
                app->getMouse().getScrollOffset().y * player->fovSensitivity;
    fov = glm::clamp(fov, glm::pi<float>() * 0.01f,
                     glm::pi<float>() * 0.99f); // We keep the fov in the range
                                                // 0.01*PI to 0.99*PI
    camera->fovY = fov;
    float speed = 2.0;

    // We get the camera model matrix (relative to its parent) to
    // compute the front, up and right directions
    glm::mat4 matrix = entity->localTransform.toMat4();

    glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
              up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)),
              right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

    glm::vec3 current_sensitivity = player->positionSensitivity;
    // If the LEFT SHIFT key is pressed, we multiply the position
    // sensitivity by the speed up factor
    if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT)) {
      current_sensitivity *= player->speedupFactor;
      speed *= 2;
    }

    glm::mat3 lock_y = glm::mat3(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                 1.0f); // don't change y value

    // We change the camera position based on the keys WASD/QE
    // S & W moves the player back and forth
    // if (app->getKeyboard().isPressed(GLFW_KEY_W))
    //   position +=
    //       glm::normalize(lock_y * front) * (deltaTime *
    //       current_sensitivity.z);
    // if (app->getKeyboard().isPressed(GLFW_KEY_S))
    //   position -=
    //       glm::normalize(lock_y * front) * (deltaTime *
    //       current_sensitivity.z);
    // // Q & E moves the player up and down
    // if (app->getKeyboard().isPressed(GLFW_KEY_Q))
    //   position += up * (deltaTime * current_sensitivity.y);
    // if (app->getKeyboard().isPressed(GLFW_KEY_E))
    //   position -= up * (deltaTime * current_sensitivity.y);
    // // A & D moves the player left or right
    // if (app->getKeyboard().isPressed(GLFW_KEY_D))
    //   position +=
    //       glm::normalize(lock_y * right) * (deltaTime *
    //       current_sensitivity.x);
    // if (app->getKeyboard().isPressed(GLFW_KEY_A))
    //   position -=
    //       glm::normalize(lock_y * right) * (deltaTime *
    //       current_sensitivity.x);
    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

    if (app->getKeyboard().isPressed(GLFW_KEY_W))
      velocity += glm::normalize(lock_y * front);
    if (app->getKeyboard().isPressed(GLFW_KEY_S))
      velocity -= glm::normalize(lock_y * front);
    // Q & E moves the player up and down
    if (app->getKeyboard().isPressed(GLFW_KEY_Q))
      position += up * (deltaTime * current_sensitivity.y);
    if (app->getKeyboard().isPressed(GLFW_KEY_E))
      position -= up * (deltaTime * current_sensitivity.y);
    // A & D moves the player left or right
    if (app->getKeyboard().isPressed(GLFW_KEY_D))
      velocity += glm::normalize(lock_y * right);
    if (app->getKeyboard().isPressed(GLFW_KEY_A))
      velocity -= glm::normalize(lock_y * right);
    if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1)) {
      if (time > lastShot + fireRate) {
        auto spawner = entity->getComponent<SpawnerComponent>();
        if (spawner) {
          auto bullet = spawner->spawn(world);
          auto bulletdata = bullet->getComponent<BulletComponent>();
          bulletdata->direction = front;
          bullet->localTransform.setPosition(
              glm::mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0,
                        0.0, front.x * 2, front.y * 2, front.z * 2, 1.0) *
              player->getOwner()->getLocalToWorldMatrix());
          bulletdata->shotTime = time;
          lastShot = time;
        }
      }
    }
    if (app->getKeyboard().isPressed(GLFW_KEY_SPACE) &&
        entity->getComponent<PlayerComponent>()->onTheGround) {
      rigidbody->rigidbody->applyWorldForceAtCenterOfMass(
          reactphysics3d::Vector3(0.0f, 400.0f, 0.0f));
      entity->getComponent<PlayerComponent>()->onTheGround = false;
    }

    if (rigidbody && entity->getComponent<PlayerComponent>()->onTheGround) {
      rigidbody->rigidbody->setAngularLockAxisFactor(
          reactphysics3d::Vector3(0, 0, 0));
      rigidbody->rigidbody->getCollider(0)->getMaterial().setBounciness(0);
      reactphysics3d::Vector3 temp = convert(velocity) * speed;
      temp.y = rigidbody->rigidbody->getLinearVelocity().y;

      rigidbody->rigidbody->setLinearVelocity(temp);
    }
    if (player->currentHealth == 0) {
      app->changeState("menu");
    }
  }

  // When the state exits, it should call this function to ensure the mouse is
  // unlocked
  void exit() {
    if (mouse_locked) {
      mouse_locked = false;
      app->getMouse().unlockMouse(app->getWindow());
    }
  }
};

} // namespace our
