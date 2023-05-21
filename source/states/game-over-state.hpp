#pragma once

#include <application.hpp>
#include <material/material.hpp>
#include <mesh/mesh.hpp>
#include <shader/shader.hpp>
#include <texture/texture-utils.hpp>
#include <texture/texture2d.hpp>

#include <SFML/Audio.hpp>
#include <array>
#include <functional>

// This state shows how to use some of the abstractions we created to make a
// menu.
class GameOverstate : public our::State {

  // A meterial holding the menu shader and the menu texture to draw
  our::TexturedMaterial *menuMaterial;
  // A material to be used to highlight hovered buttons (we will use blending to
  // create a negative effect).
  our::TintedMaterial *highlightMaterial;
  // A rectangle mesh on which the menu material will be drawn
  our::Mesh *rectangle;
  // A variable to record the time since the state is entered (it will be used
  // for the fading effect).
  float time;
  // An array of the button that we can interact with
  sf::Music *music = nullptr;

  void onInitialize() override {
    // First, we create a material for the menu's background
    menuMaterial = new our::TexturedMaterial();
    // Here, we load the shader that will be used to draw the background
    menuMaterial->shader = new our::ShaderProgram();
    menuMaterial->sampler = new our::Sampler(); // add a sampler to the material
    menuMaterial->shader->attach("assets/shaders/textured.vert",
                                 GL_VERTEX_SHADER);
    menuMaterial->shader->attach("assets/shaders/textured.frag",
                                 GL_FRAGMENT_SHADER);
    menuMaterial->shader->link();
    // Then we load the menu texture
    menuMaterial->texture =
        our::texture_utils::loadImage("assets/textures/GameOver.png");
    // Initially, the menu material will be black, then it will fade in
    menuMaterial->tint = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    // Then we create a rectangle whose top-left corner is at the origin and its
    // size is 1x1. Note that the texture coordinates at the origin is
    // (0.0, 1.0) since we will use the projection matrix to make the origin at
    // the the top-left corner of the screen.
    rectangle = new our::Mesh(
        {
            {{0.0f, 0.0f, 0.0f},
             {255, 255, 255, 255},
             {0.0f, 1.0f},
             {0.0f, 0.0f, 1.0f}},
            {{1.0f, 0.0f, 0.0f},
             {255, 255, 255, 255},
             {1.0f, 1.0f},
             {0.0f, 0.0f, 1.0f}},
            {{1.0f, 1.0f, 0.0f},
             {255, 255, 255, 255},
             {1.0f, 0.0f},
             {0.0f, 0.0f, 1.0f}},
            {{0.0f, 1.0f, 0.0f},
             {255, 255, 255, 255},
             {0.0f, 0.0f},
             {0.0f, 0.0f, 1.0f}},
        },
        {
            0,
            1,
            2,
            2,
            3,
            0,
        });

    // Reset the time elapsed since the state is entered.
    time = 0;
    music = new sf::Music;
    music->openFromFile("assets/audio/Game Over.ogg");
    music->setPitch(1.0f);
    music->setVolume(100.0f);
    music->setLoop(true);
    music->play();
  }

  void onDraw(double deltaTime) override {
    // Get the framebuffer size to set the viewport and the create the
    // projection matrix.
    glm::ivec2 size = getApp()->getFrameBufferSize();
    // Make sure the viewport covers the whole size of the framebuffer.
    glViewport(0, 0, size.x, size.y);

    auto &keyboard = getApp()->getKeyboard();

    if (keyboard.justPressed(GLFW_KEY_SPACE)) {
      // If the space key is pressed in this frame, go to the play state
      getApp()->changeState("menu");
    }

    // The view matrix is an identity (there is no camera that moves around).
    // The projection matrix applys an orthographic projection whose size is
    // the framebuffer size in pixels so that the we can define our object
    // locations and sizes in pixels. Note that the top is at 0.0 and the
    // bottom is at the framebuffer height. This allows us to consider the
    // top-left corner of the window to be the origin which makes dealing with
    // the mouse input easier.
    glm::mat4 VP =
        glm::ortho(0.0f, (float)size.x, (float)size.y, 0.0f, 1.0f, -1.0f);
    // The local to world (model) matrix of the background which is just a
    // scaling matrix to make the menu cover the whole window. Note that we
    // defind the scale in pixels.
    glm::mat4 M = glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

    // First, we apply the fading effect.
    time += (float)deltaTime;
    menuMaterial->tint = glm::vec4(glm::smoothstep(0.00f, 2.00f, time));
    // Then we render the menu background
    // Notice that I don't clear the screen first, since I assume that the
    // menu rectangle will draw over the whole window anyway.
    menuMaterial->setup();
    menuMaterial->shader->set("transform", VP * M);
    rectangle->draw();
    if (time > 26) {
      getApp()->changeState("menu");
    }

    // For every button, check if the mouse is inside it. If the mouse is
    // inside, we draw the highlight rectangle over it.
  }

  void onDestroy() override {
    // Delete all the allocated resources
    delete rectangle;
    delete menuMaterial->texture;
    delete menuMaterial->shader;
    delete menuMaterial;
    delete music;
  }
};