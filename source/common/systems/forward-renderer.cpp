#include "forward-renderer.hpp"
#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"
#include <components/player.hpp>
#include <string>

namespace our {
void ForwardRenderer::initialize(glm::ivec2 windowSize,
                                 const nlohmann::json &config) {
  // First, we store the window size for later use
  this->windowSize = windowSize;

  // Then we check if there is a sky texture in the configuration
  if (config.contains("sky")) {
    // First, we create a sphere which will be used to draw the sky
    this->skySphere = mesh_utils::sphere(glm::ivec2(16, 16));

    // We can draw the sky using the same shader used to draw textured objects
    ShaderProgram *skyShader = new ShaderProgram();
    skyShader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
    skyShader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
    skyShader->link();

    // TODO: (Req 10) Pick the correct pipeline state to draw the sky
    //  Hints: the sky will be draw after the opaque objects so we would need
    //  depth testing but which depth funtion should we pick? We will draw the
    //  sphere from the inside, so what options should we pick for the face
    //  culling.
    PipelineState skyPipelineState{};
    // set the depth testing to true
    skyPipelineState.depthTesting.enabled = true;
    // set the depth function to GL_LEQUAL
    skyPipelineState.depthTesting.function = GL_LEQUAL;
    // set the face culling to true
    skyPipelineState.faceCulling.enabled = true;
    // set the culled face to GL_FRONT
    skyPipelineState.faceCulling.culledFace = GL_FRONT;

    // Load the sky texture (note that we don't need mipmaps since we want
    // to avoid any unnecessary blurring while rendering the sky)
    std::string skyTextureFile = config.value<std::string>("sky", "");
    Texture2D *skyTexture = texture_utils::loadImage(skyTextureFile, false);

    // Setup a sampler for the sky
    Sampler *skySampler = new Sampler();
    skySampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    skySampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    skySampler->set(GL_TEXTURE_WRAP_S, GL_REPEAT);
    skySampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Combine all the aforementioned objects (except the mesh) into a material
    this->skyMaterial = new TexturedMaterial();
    this->skyMaterial->shader = skyShader;
    this->skyMaterial->texture = skyTexture;
    this->skyMaterial->sampler = skySampler;
    this->skyMaterial->pipelineState = skyPipelineState;
    this->skyMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    this->skyMaterial->alphaThreshold = 1.0f;
    this->skyMaterial->transparent = false;
  }

  // Then we check if there is a postprocessing shader in the configuration
  if (config.contains("postprocess")) {
    // TODO: (Req 11) Create a framebuffer
    // generate FrameBuffer
    glGenFramebuffers(1, &postprocessFrameBuffer);
    // Bind the FrameBuffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postprocessFrameBuffer);

    // TODO: (Req 11) Create a color and a depth texture and attach them to the
    // framebuffer
    //  Hints: The color format can be (Red, Green, Blue and Alpha components
    //  with 8 bits for each channel). The depth format can be (Depth component
    //  with 24 bits).

    // create empty (RGB) color
    colorTarget = texture_utils::empty(GL_RGBA8, windowSize);
    // attach the color to the frame Buffer
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, colorTarget->getOpenGLName(), 0);
    // create empty depth
    depthTarget = texture_utils::empty(GL_DEPTH_COMPONENT24, windowSize);
    // attach the depth to the frame Buffer
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, depthTarget->getOpenGLName(), 0);

    // TODO: (Req 11) Unbind the framebuffer just to be safe
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    // Create a vertex array to use for drawing the texture
    glGenVertexArrays(1, &postProcessVertexArray);

    // Create a sampler to use for sampling the scene texture in the post
    // processing shader
    Sampler *postprocessSampler = new Sampler();
    postprocessSampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    postprocessSampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    postprocessSampler->set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    postprocessSampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Create the post processing shader
    ShaderProgram *postprocessShader = new ShaderProgram();
    postprocessShader->attach("assets/shaders/fullscreen.vert",
                              GL_VERTEX_SHADER);
    postprocessShader->attach(config.value<std::string>("postprocess", ""),
                              GL_FRAGMENT_SHADER);
    postprocessShader->link();

    // Create a post processing material
    postprocessMaterial = new TexturedMaterial();
    postprocessMaterial->shader = postprocessShader;
    postprocessMaterial->texture = colorTarget;
    postprocessMaterial->sampler = postprocessSampler;
    // The default options are fine but we don't need to interact with the depth
    // buffer so it is more performant to disable the depth mask
    postprocessMaterial->pipelineState.depthMask = false;

    // Create a post processing material
    postprocessSampler = new Sampler();
    postprocessSampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    postprocessSampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    postprocessSampler->set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    postprocessSampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Create the post processing shader
    postprocessShader = new ShaderProgram();
    postprocessShader->attach("assets/shaders/fullscreen.vert",
                              GL_VERTEX_SHADER);
    postprocessShader->attach("assets/shaders/postprocess/rededges.frag",
                              GL_FRAGMENT_SHADER);
    postprocessShader->link();

    // Create a post processing material
    HitPostprocessMaterial = new TexturedMaterial();
    HitPostprocessMaterial->shader = postprocessShader;
    HitPostprocessMaterial->texture = colorTarget;
    HitPostprocessMaterial->sampler = postprocessSampler;
    // The default options are fine but we don't need to interact with the depth
    // buffer so it is more performant to disable the depth mask
    HitPostprocessMaterial->pipelineState.depthMask = false;
  }
}

void ForwardRenderer::destroy() {
  // Delete all objects related to the sky
  if (skyMaterial) {
    delete skySphere;
    delete skyMaterial->shader;
    delete skyMaterial->texture;
    delete skyMaterial->sampler;
    delete skyMaterial;
  }
  // Delete all objects related to post processing
  if (postprocessMaterial) {
    glDeleteFramebuffers(1, &postprocessFrameBuffer);
    glDeleteVertexArrays(1, &postProcessVertexArray);
    delete colorTarget;
    delete depthTarget;
    delete postprocessMaterial->sampler;
    delete postprocessMaterial->shader;
    delete postprocessMaterial;
  }
}

void ForwardRenderer::render(World *world, std::vector<our::Light> light_list,
                             float deltaTime) {
  // First of all, we search for a camera and for all the mesh renderers
  CameraComponent *camera = nullptr;
  opaqueCommands.clear();
  transparentCommands.clear();
  for (auto entity : world->getEntities()) {
    // If we hadn't found a camera yet, we look for a camera in this entity
    if (!camera)
      camera = entity->getComponent<CameraComponent>();
    // If this entity has a mesh renderer component
    if (auto meshRenderer = entity->getComponent<MeshRendererComponent>();
        meshRenderer) {
      // We construct a command from it
      RenderCommand command;
      command.localToWorld = meshRenderer->getOwner()->getLocalToWorldMatrix();
      command.center = glm::vec3(command.localToWorld * glm::vec4(0, 0, 0, 1));
      command.mesh = meshRenderer->mesh;
      command.material = meshRenderer->material;
      // if it is transparent, we add it to the transparent commands list
      if (command.material->transparent) {
        transparentCommands.push_back(command);
      } else {
        // Otherwise, we add it to the opaque command list
        opaqueCommands.push_back(command);
      }
    }
  }

  // If there is no camera, we return (we cannot render without a camera)
  if (camera == nullptr)
    return;

  // TODO: (Req 9) Modify the following line such that "cameraForward" contains
  // a vector pointing the camera forward direction
  //  HINT: See how you wrote the CameraComponent::getViewMatrix, it should help
  //  you solve this one
  glm::vec3 cameraForward =
      glm::vec3(camera->getOwner()->getLocalToWorldMatrix() *
                glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); // get camera forward
  std::sort(
      transparentCommands.begin(), transparentCommands.end(),
      [cameraForward](const RenderCommand &first, const RenderCommand &second) {
        // TODO: (Req 9) Finish this function
        //  HINT: the following return should return true "first"
        //  should be drawn before "second".
        return (glm::dot(cameraForward, first.center) >
                glm::dot(cameraForward,
                         second.center)); // compare the two distances
      });

  // TODO: (Req 9) Get the camera ViewProjection matrix and store it in VP
  glm::mat4 V = camera->getViewMatrix(); // get the camera view matrix
  glm::mat4 P = camera->getProjectionMatrix(glm::ivec2(
      windowSize.x, windowSize.y)); // get the camera projection matrix
  glm::mat4 VP = P * V;             // get VP

  // TODO: (Req 9) Set the OpenGL viewport using viewportStart and viewportSize
  glViewport(0, 0, windowSize.x, windowSize.y); // set the view port
  // TODO: (Req 9) Set the clear color to black and the clear depth to
  // 1
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // set the clear color
  glClearDepth(1);                      // set the clear depth

  // TODO: (Req 9) Set the color mask to true and the depth mask to
  // true (to ensure the glClear will affect the framebuffer)
  glColorMask(true, true, true, true); // set the color mask
  glDepthMask(true);                   // set the depth mask

  // If there is a postprocess material, bind the framebuffer
  if (postprocessMaterial) {
    // TODO: (Req 11) bind the framebuffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postprocessFrameBuffer);
  }

  // TODO: (Req 9) Clear the color and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // TODO: (Req 9) Draw all the opaque commands
  //  Don't forget to set the "transform" uniform to be equal the
  //  model-view-projection matrix for each render command
  //  loop over all opaque objects

  // TODO: (Req 10) Get the camera position
  glm::vec4 cameraPosition = camera->getOwner()->getLocalToWorldMatrix() *
                             glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  for (auto x : opaqueCommands) {
    our::Material *material = x.material;          // A pointer to the material
    our::ShaderProgram *shader = material->shader; // A pointer to the shader
    our::LitMaterial *litMaterial = dynamic_cast<our::LitMaterial *>(material);
    material->setup(); // setup the material
    if (litMaterial) {
      for (int i = 0; i < light_list.size(); i++) {
        std::string uf_loc = "lights[" + std::to_string(i) + "]";
        shader->set((uf_loc) + ".type", light_list[i].type);
        shader->set((uf_loc) + ".color", light_list[i].color);
        shader->set((uf_loc) + ".position", light_list[i].position);
        shader->set((uf_loc) + ".direction", light_list[i].direction);
        shader->set((uf_loc) + ".attenuation", light_list[i].attenuation);
        shader->set((uf_loc) + ".cone_angles", light_list[i].cone_angles);
      }
      shader->set("light_count", (int)light_list.size());
      shader->set("sky.top", glm::vec3(0.0f, 0.1f, 0.5f));
      shader->set("sky.horizon", glm::vec3(0.3f, 0.3f, 0.3f));
      shader->set("sky.bottom", glm::vec3(0.1f, 0.1f, 0.1f));
      glm::mat4 M = x.localToWorld;
      glm::mat4 M_IT = glm::transpose(glm::inverse(M));
      shader->set("M", M);
      shader->set("M_IT", M_IT);
      shader->set("VP", VP);
      shader->set("camera_position", glm::vec3(cameraPosition));
    } else {
      shader->set("transform",
                  (VP * x.localToWorld)); // set the transform to the shader
    }

    x.mesh->draw(); // draw the mesh
  }

  // If there is a sky material, draw the sky
  if (this->skyMaterial) {
    // TODO: (Req 10) setup the sky material
    this->skyMaterial->setup();

    // TODO: (Req 10) Create a model matrix for the sy such
    // that it always follows the camera (sky sphere center =
    // camera position)
    glm::mat4 M = glm::mat4(
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        cameraPosition.x, cameraPosition.y, cameraPosition.z, cameraPosition.w);
    // TODO: (Req 10) We want the sky to be drawn behind everything (in
    // NDC space, z=1)
    //  We can acheive the is by multiplying by an extra matrix after
    //  the projection but what values should we put in it?
    glm::mat4 alwaysBehindTransform = glm::mat4(
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 1.0f); // A transformation matrix to set the third
                                 // row(Z) to be the same as the fourth row(W)
    glm::mat4 transform_M = alwaysBehindTransform * VP * M;
    // TODO: (Req 10) set the "transform" uniform
    this->skyMaterial->shader->set("transform", (transform_M));
    // TODO: (Req 10) draw the sky sphere
    this->skySphere->draw();
  }
  // TODO: (Req 9) Draw all the transparent commands
  //  Don't forget to set the "transform" uniform to be equal the
  //  model-view-projection matrix for each render command
  //  loop over all transparent objects
  for (auto x : transparentCommands) {
    our::Material *material = x.material;          // A pointer to the material
    our::ShaderProgram *shader = material->shader; // A pointer to the shader
    our::LitMaterial *litMaterial = dynamic_cast<our::LitMaterial *>(material);
    material->setup(); // setup the material
    if (litMaterial) {
      for (int i = 0; i < light_list.size(); i++) {
        std::string uf_loc = "lights[" + std::to_string(i) + "]";
        shader->set((uf_loc) + ".type", light_list[i].type);
        shader->set((uf_loc) + ".color", light_list[i].color);
        shader->set((uf_loc) + ".position", light_list[i].position);
        shader->set((uf_loc) + ".direction", light_list[i].direction);
        shader->set((uf_loc) + ".attenuation", light_list[i].attenuation);
        shader->set((uf_loc) + ".cone_angles", light_list[i].cone_angles);
      }
      shader->set("light_count", (int)light_list.size());
      shader->set("sky.top", glm::vec3(0.0f, 0.1f, 0.5f));
      shader->set("sky.horizon", glm::vec3(0.3f, 0.3f, 0.3f));
      shader->set("sky.bottom", glm::vec3(0.1f, 0.1f, 0.1f));
      glm::mat4 M = x.localToWorld;
      glm::mat4 M_IT = glm::transpose(glm::inverse(M));
      shader->set("M", M);
      shader->set("M_IT", M_IT);
      shader->set("VP", VP);
      shader->set("camera_position", glm::vec3(cameraPosition));
    } else {
      shader->set("transform",
                  (VP * x.localToWorld)); // set the transform to the shader
    }

    x.mesh->draw(); // draw the mesh
  }

  // If there is a postprocess material, apply postprocessing
  if (postprocessMaterial) {
    // TODO: (Req 11) Return to the default framebuffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    // TODO: (Req 11) Setup the postprocess material and draw the fullscreen
    // triangle
    // setup
    int health =
        camera->getOwner()->getComponent<PlayerComponent>()->currentHealth;
    if (health <
        camera->getOwner()->getComponent<PlayerComponent>()->maxHealth) {
      HitPostprocessMaterial->setup();
      HitPostprocessMaterial->shader->set("health", (float)health);
    } else {
      postprocessMaterial->setup();
    }
    // bind the postProcess
    glBindVertexArray(postProcessVertexArray);
    // draw
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
}

} // namespace our