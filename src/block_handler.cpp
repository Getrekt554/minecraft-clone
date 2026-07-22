#include "block_handler.hpp"
#include "renderer.hpp"

void add_block(glm::vec3 pos, std::vector<float> &vertices,
               std::vector<unsigned int> &indices, unsigned int texture) {

  glm::vec2 uv_offset = get_texture_offset(texture);

  std::vector<unsigned int> cube_indices = {
      1,  0,  2,  2,  0,  3,  // Back
      4,  5,  6,  4,  6,  7,  // Front
      8,  9,  10, 8,  10, 11, // Left
      13, 12, 14, 14, 12, 15, // Right
      16, 17, 18, 16, 18, 19, // Bottom
      21, 20, 22, 22, 20, 23  // Top
  };
  std::vector<float> cube_vertices = {
      // Positions          // Colors           // Texture Coords
      // Back Face
      -0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
      0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

      // Front Face
      -0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
      0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

      // Left Face
      -0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      -0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
      -0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

      // Right Face
      0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
      0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

      // Bottom Face
      -0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      -0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

      // Top Face
      -0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      -0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f};

  for (unsigned int &index : cube_indices) {
    index += vertices.size() / 8;
  }

  for (int i = 6; i < cube_vertices.size(); i += 7) {
    cube_vertices.at(i) /= ATLAS_SIZE;
    cube_vertices.at(i++) += uv_offset.x;
    cube_vertices.at(i) /= ATLAS_SIZE;
    cube_vertices.at(i) += uv_offset.y;
  }

  vertices.insert(vertices.end(), cube_vertices.begin(), cube_vertices.end());

  indices.insert(indices.end(), cube_indices.begin(), cube_indices.end());
}
