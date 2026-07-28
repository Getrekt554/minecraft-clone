#pragma once

#include "world.hpp"
#include <cmath>
#include <vector>
#include "block_handler.hpp"

struct ObjData {
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
};


enum class DIRECTIONS { NORTH, SOUTH, EAST, WEST, UP, DOWN, UNDEFINED };

struct raycast_data {
  bool success;
  Vector3i position;
  DIRECTIONS block_face;
};

Vector3i face_to_offset(DIRECTIONS face);
DIRECTIONS offset_to_face(Vector3i offset);

Vector3 normalize(Vector3 vector);

bool block_face_covered(WorldManager *world, Vector3i position,
                        DIRECTIONS face);

// IF THE RAY EVER HAS TARGET ERRORS, ITS BECAUSE THE CONVERSION FROM RAY
// POSITION TO BLOCK POSITION IS WRONG
raycast_data cast_ray(WorldManager *world, Vector3 position, Vector3 direction);

void chunk_data(chunk target_chunk, ObjData& output);
void append_chunk_data(ObjData& mesh, ObjData data);
