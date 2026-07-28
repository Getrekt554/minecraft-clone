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

  std::vector<unsigned int> cube_indices;// = {
  //     1,  0,  2,  2,  0,  3,  // Back
  //     4,  5,  6,  4,  6,  7,  // Front
  //     8,  9,  10, 8,  10, 11, // Left
  //     13, 12, 14, 14, 12, 15, // Right
  //     16, 17, 18, 16, 18, 19, // Bottom
  //     21, 20, 22, 22, 20, 23  // Top
  // };

  

  std::vector<float> cube_vertices ;//= {
  //     // Positions          // Colors           // Texture Coords
  //     // Back Face
  //     -0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  //     0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
  //     0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  //     -0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

  //     // Front Face
  //     -0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  //     0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
  //     0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  //     -0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

  //     // Left Face
  //     -0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  //     -0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
  //     -0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  //     -0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

  //     // Right Face
  //     0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  //     0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
  //     0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  //     0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

  //     // Bottom Face
  //     -0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
  //     0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  //     0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
  //     -0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

  //     // Top Face
  //     -0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
  //     0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  //     0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
  //     -0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f};

  if (!block_face_covered(world, pos, DIRECTIONS::NORTH)) {
    cube_vertices.insert(cube_vertices.end(), {-0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
      0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f});
    cube_indices.insert(cube_indices.end(),
      {1,  0,  2,  2,  0,  3});
  }
  else {
    o = 4;
  }
  if (!block_face_covered(world, pos, DIRECTIONS::SOUTH)) {
    cube_vertices.insert(cube_vertices.end(), {-0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
      0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f});
    cube_indices.insert(cube_indices.end(),
      {4-o,  5-o,  6-o,  4-o,  6-o,  7-o});
  }
  else {
    o += 4;
  }
  if (!block_face_covered(world, pos, DIRECTIONS::WEST)) {
    cube_vertices.insert(cube_vertices.end(), {-0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      -0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
      -0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f});
    cube_indices.insert(cube_indices.end(),
      {8-o,  9-o,  10-o, 8-o,  10-o, 11-o});
  }
  else {
    o += 4;
  }
  if (!block_face_covered(world, pos, DIRECTIONS::EAST)) {
    cube_vertices.insert(cube_vertices.end(), {0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
      0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f});
    cube_indices.insert(cube_indices.end(),
      {13-o, 12-o, 14-o, 14-o, 12-o, 15-o});
  }
  else {
    o += 4;
  }
  if (!block_face_covered(world, pos, DIRECTIONS::DOWN)) {
    cube_vertices.insert(cube_vertices.end(), {-0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      -0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f});
    cube_indices.insert(cube_indices.end(),
      {16-o, 17-o, 18-o, 16-o, 18-o, 19-o});
  }
  else {
    o += 4;
  }
  if (!block_face_covered(world, pos, DIRECTIONS::UP)) {
    cube_vertices.insert(cube_vertices.end(), {-0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      -0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f});
    cube_indices.insert(cube_indices.end(),
      {21-o, 20-o, 22-o, 22-o, 20-o, 23-o});
  }
  else {
    o += 4;
  }

  for (unsigned int &index : cube_indices) {
    index += vertices.size() / 8;
  }

  for (int i = 6; i < cube_vertices.size(); i += 6) {
    cube_vertices[i] += uv_offset.x;
    cube_vertices[i++] /= ATLAS_SIZE;
    cube_vertices[i] += uv_offset.y;
    cube_vertices[i++] /= ATLAS_SIZE;
  }

  vertices.insert(vertices.end(), cube_vertices.begin(), cube_vertices.end());

  indices.insert(indices.end(), cube_indices.begin(), cube_indices.end());
}
