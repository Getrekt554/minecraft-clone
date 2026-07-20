#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../external/glm/glm.hpp"
#include "../external/glm/gtc/matrix_transform.hpp"
#include "../external/glm/gtc/type_ptr.hpp"

class Camera {
public:
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;

  float speed = 8.0f;
  float sensitivity = 300.0f;

  float Yaw;
  float Pitch;
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;

  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f,
         float pitch = 0.0f);

  glm::mat4 get_view_matrix() const;

  void move(glm::vec3 direction, float distance);

  void rotate(float xoffset, float yoffset);

  void tick(unsigned int shader_program);

private:
  void update_data();
};
