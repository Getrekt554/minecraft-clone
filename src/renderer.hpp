#pragma once
#include <iostream>
#include "../external/glm/glm.hpp"
#include "../external/glm/gtc/matrix_transform.hpp"
#include "../external/glm/gtc/type_ptr.hpp"
#include "camera.hpp"

class Renderer {
public:
  void init();

  void tick(Camera camera);

  void draw();

private:
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
  unsigned int vertex_shader;
  unsigned int frag_shader;
  unsigned int shader_program;
  unsigned int texture;
};
