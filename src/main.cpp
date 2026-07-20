#include <iostream>
#include "renderer.hpp"

#define PI 3.14159265359

const int WIDTH = 800;
const int HEIGHT = 800;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

Camera player_camera(glm::vec3(0.0f, 0.0f, 4.0f));

double last_mouse_x = 400.0f;
double last_mouse_y = 400.0f;
bool first_mouse = true;
void process_input(GLFWwindow *window, float delta_time) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  // camera movement
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    player_camera.move(player_camera.Front, player_camera.speed * delta_time);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    player_camera.move(-player_camera.Front, player_camera.speed * delta_time);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    player_camera.move(-player_camera.Right, player_camera.speed * delta_time);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    player_camera.move(player_camera.Right, player_camera.speed * delta_time);

  // camera rotation
  double mouse_x, mouse_y;
  glfwGetCursorPos(window, &mouse_x, &mouse_y);

  // first mouse sets the mouse_pos at startup
  if (first_mouse) {
    last_mouse_x = mouse_x;
    last_mouse_y = mouse_y;
    first_mouse = false;
  }

  float x_offset = (float)(mouse_x - last_mouse_x);
  float y_offset = (float)(last_mouse_y - mouse_y);

  last_mouse_x = mouse_x;
  last_mouse_y = mouse_y;

  player_camera.rotate(x_offset * player_camera.sensitivity * delta_time,
                       y_offset * player_camera.sensitivity * delta_time);
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 800, "CloneCraft", NULL, NULL);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (window == NULL) {
    std::cout << "Failed to create window\n";
    exit(-1);
  }
  glfwMakeContextCurrent(window);

  // glfwSwapInterval(1); // vsync

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to init GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, WIDTH, HEIGHT);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glEnable(GL_DEPTH_TEST);

  Renderer renderer;

  renderer.init();
  player_camera.model = glm::mat4(1.0f);

  float delta_time = 0.0f;
  float last_frame = 0.0f;

  while (!glfwWindowShouldClose(window)) {
    //calculate delta time
    float current_frame = (float)glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    // input
    process_input(window, delta_time);

    player_camera.model = glm::rotate(player_camera.model, glm::radians(15.0f) * delta_time,
                                      glm::vec3(0.5f, 1.0f, 0.0f));
    renderer.tick(player_camera);

    renderer.draw();

    // check events and swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
