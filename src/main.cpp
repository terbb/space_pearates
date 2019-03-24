#ifndef MAIN_H
#define MAIN_H

#define GL3W_IMPLEMENTATION  // DO NOT define this in any other C++ or header files
#define STB_IMAGE_IMPLEMENTATION // DO NOT define this in any other C++ or header files
#include "Utility/utility.hpp"
#include "world.hpp"

GLboolean keys[1024];
GLboolean keysProcessed[1024];

class Main{
public:
  ~Main(){
    glfwTerminate();
  }
};
shared_ptr<Main> mainObj = make_shared<Main>();
shared_ptr<World> world;

void error_callback(int error, const char* desc)
{
  fprintf(stderr, "%d: %s", error, desc);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (key >= 0 && key < 1024)
  {
    if (action == GLFW_PRESS) {
      keys[key] = GL_TRUE;
    }
    else if (action == GLFW_RELEASE) {
      keys[key] = GL_FALSE;
      keysProcessed[key] = GL_FALSE;
    }
  }
}

int main(int argc, char * argv[]) {
  // Set our error callback function, which will print any glfw-related errors.
  // This error callback may only catch errors that we specifically throw - so this may be redundant.
  glfwSetErrorCallback(error_callback);

  // Load GLFW which handles all of the extraneous stuff like creating a window
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW.");
    return EXIT_FAILURE;
  }

  // The options we want to configure when creating a window using glfw.
  // We want to use OpenGL 3.3, as we want to use modern OpenGL (aka Core-Profile)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  // Opens up a debug window for error/performance-issue related reporting. Set to GL_FALSE if you don't want it to open.
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  // Create a GLFWwindow, which encapsulates a window and an OpenGL context.
  auto mWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "436D_Project", nullptr, nullptr);
  if (mWindow == nullptr) {
    fprintf(stderr, "Failed to create GLFWwindow (window + OpenGL context).");
    return EXIT_FAILURE;
  }
  // Set the current OpenGL context to this window.
  glfwMakeContextCurrent(mWindow);
  // For vsync. Locks the frame draw rate to the frame refresh rate of your monitor to avoid artifacts.
  glfwSwapInterval(1);

  // Load OpenGL function pointers.
  if (gl3w_init() == -1) {
    fprintf(stderr, "Failed to initialize gl3w.");
  }

  // Check which version of OpenGL we're on.
  fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

  // Set our key_callback function, which handles all keyboard-related inputs.
  glfwSetKeyCallback(mWindow, key_callback);

  // Configure OpenGL.
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  // glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Playing background music indefinitely (init audio)
  AudioLoader::getInstance();

  world = make_shared<World>();
  world->init(vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

  // dT variables.
  GLfloat deltaTime = 0.0f;
  GLfloat lastFrame = glfwGetTime();
  bool paused = false;

  // Rendering loop.
  while (!glfwWindowShouldClose(mWindow)) {
    // Clear our screen for the upcoming render call.
    // TO-DO: Do we need to deal with depth?
    // TO-DO: Do we need a projection matrix?
    // TO-DO: Do we need to render to a custom framebuffer? (likely not)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Calculate dT.
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Process events from the window system.
    glfwPollEvents();

    if (keys[GLFW_KEY_R] && !keysProcessed[GLFW_KEY_R])
    {
      world->reset();
      paused = false;
      HelpMenu::getInstance().showHelp = paused;
      keysProcessed[GLFW_KEY_R] = true;
    }
    if (keys[GLFW_KEY_M] && !keysProcessed[GLFW_KEY_M])
    {
      AudioLoader::getInstance().changeBgm();
      keysProcessed[GLFW_KEY_M] = true;
    }
    // Launch help texture == pause
    if (keys[GLFW_KEY_H] && !keysProcessed[GLFW_KEY_H] 
      ||keys[GLFW_KEY_P] && !keysProcessed[GLFW_KEY_P])
    {
      paused = !paused;
      HelpMenu::getInstance().showHelp = paused;
      keysProcessed[GLFW_KEY_H] = true;
      keysProcessed[GLFW_KEY_P] = true;
    }

    if (!paused) {
      world->processInput(deltaTime, keys, keysProcessed);
      world->update(deltaTime);
    }
    world->draw();

    // Flip buffers and draw.
    glfwSwapBuffers(mWindow);
  }

  AudioLoader::getInstance().destroy();

  return EXIT_SUCCESS;
}

#endif
