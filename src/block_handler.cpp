#include "block_handler.hpp"
#include "renderer.hpp"
#include <cstdint>

void add_block(glm::vec3 pos, std::vector<float> &vertices,
               std::vector<unsigned int> &indices, uint8_t texture, WorldManager *world) {
  if (texture == uint8_t(BLOCK::AIR)) {
    return;
  }
  
  unsigned int o = 0;

  glm::vec2 uv_offset = get_texture_offset(texture - 1);

  std::vector<unsigned int> cube_indices;

  std::vector<float> cube_vertices ;

  if (!block_face_covered(world, pos, DIRECTIONS::NORTH)) {
    cube_vertices.insert(cube_vertices.end(), {-0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.8f, 0.8f, 0.8f,
      0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.8f, 0.8f, 0.8f,
      0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.8f, 0.8f,
      -0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.8f, 0.8f, 0.8f,});
    cube_indices.insert(cube_indices.end(),
      {1,  0,  2,  2,  0,  3});
  }
  else {
    o = 4;
  }
  if (!block_face_covered(world, pos, DIRECTIONS::SOUTH)) {
    cube_vertices.insert(cube_vertices.end(), {-0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.8f, 0.8f, 0.8f,
      0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.8f, 0.8f, 0.8f,
      0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.8f, 0.8f,
      -0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.8f, 0.8f, 0.8f,});
    cube_indices.insert(cube_indices.end(),
      {4-o,  5-o,  6-o,  4-o,  6-o,  7-o});
  }
  else {
    o += 4;
  }
  if (!block_face_covered(world, pos, DIRECTIONS::WEST)) {
    cube_vertices.insert(cube_vertices.end(), {-0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.65f, 0.65f, 0.65f,
      -0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.65f, 0.65f, 0.65f,
      -0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.65f, 0.65f, 0.65f,
      -0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.65f, 0.65f, 0.65f,});
    cube_indices.insert(cube_indices.end(),
      {8-o,  9-o,  10-o, 8-o,  10-o, 11-o});
  }
  else {
    o += 4;
  }
  if (!block_face_covered(world, pos, DIRECTIONS::EAST)) {
    cube_vertices.insert(cube_vertices.end(), {0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.65f, 0.65f, 0.65f,
      0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.65f, 0.65f, 0.65f,
      0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.65f, 0.65f, 0.65f,
      0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.65f, 0.65f, 0.65f,});
    cube_indices.insert(cube_indices.end(),
      {13-o, 12-o, 14-o, 14-o, 12-o, 15-o});
  }
  else {
    o += 4;
  }
  if (!block_face_covered(world, pos, DIRECTIONS::DOWN)) {
    cube_vertices.insert(cube_vertices.end(), {-0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.4f, 0.4f, 0.4f,
      0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.4f, 0.4f, 0.4f,
      0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.4f, 0.4f, 0.4f,
      -0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.4f, 0.4f, 0.4f,});
    cube_indices.insert(cube_indices.end(),
      {16-o, 17-o, 18-o, 16-o, 18-o, 19-o});
  }
  else {
    o += 4;
  }
  if (!block_face_covered(world, pos, DIRECTIONS::UP)) {
    cube_vertices.insert(cube_vertices.end(), {-0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.95f, 0.95f, 0.95f,
      0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.95f, 0.95f, 0.95f,
      0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.95f, 0.95f, 0.95f,
      -0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,});
    cube_indices.insert(cube_indices.end(),
      {21-o, 20-o, 22-o, 22-o, 20-o, 23-o});
  }
  else {
    o += 4;
  }

  for (unsigned int &index : cube_indices) {
    index += vertices.size() / 11;
  }

  if (!cube_vertices.empty()) {
    for (int i = 6; i < cube_vertices.size(); i += 11) {
      cube_vertices[i] = (cube_vertices[i] + uv_offset.x) / ATLAS_SIZE; 
      cube_vertices[i+1] = (cube_vertices[i+1] + uv_offset.y) / ATLAS_SIZE;
    }
  }

  vertices.insert(vertices.end(), cube_vertices.begin(), cube_vertices.end());

  indices.insert(indices.end(), cube_indices.begin(), cube_indices.end());
}
