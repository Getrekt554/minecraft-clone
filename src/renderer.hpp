#pragma once
#include <iostream>
#include "../external/glm/glm.hpp"
#include "../external/glm/gtc/matrix_transform.hpp"
#include "../external/glm/gtc/type_ptr.hpp"
#include "camera.hpp"
#include <vector>
#include <string>

const unsigned int ATLAS_SIZE = 16;
glm::vec2 get_texture_offset(unsigned int texture);

class Renderer {
public:
  void init();

  void tick(Camera camera);

  void draw();

  void update_buffers();
  std::vector<unsigned int> indices;
  std::vector<float> vertices;

private:
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
  unsigned int vertex_shader;
  unsigned int frag_shader;
  unsigned int shader_program;
  unsigned int texture;
};
