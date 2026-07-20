#include "camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
  Position = position;
  WorldUp = up;
  Yaw = yaw;
  Pitch = pitch;
  Front = glm::vec3(0.0f, 0.0f, -1.0f);
  update_data();
}

glm::mat4 Camera::get_view_matrix() const {
  return glm::lookAt(Position, Position + Front, Up);
}

void Camera::move(glm::vec3 direction, float distance) {
  Position += direction * distance;
}

void Camera::rotate(float xoffset, float yoffset) {
  Yaw += xoffset;
  Pitch += yoffset;

  if (Pitch > 89.0f)
    Pitch = 89.0f;
  if (Pitch < -89.0f)
    Pitch = -89.0f;

  update_data();
}

void Camera::update_data() {
  glm::vec3 front;
  front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  front.y = sin(glm::radians(Pitch));
  front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front = glm::normalize(front);

  Right = glm::normalize(glm::cross(Front, WorldUp));
  Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::tick(unsigned int shader_program) {
  // model = glm::mat4(1.0f);

  view = get_view_matrix();

  projection =
      glm::perspective(glm::radians(90.0f), 800.0f / 800.0f, 0.1f, 100.0f);

  unsigned int modelLoc = glGetUniformLocation(shader_program, "model");
  unsigned int viewLoc = glGetUniformLocation(shader_program, "view");
  unsigned int projLoc = glGetUniformLocation(shader_program, "projection");

  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}
