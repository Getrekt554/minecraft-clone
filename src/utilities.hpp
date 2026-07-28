#pragma once

#include "world.hpp"
#include <cmath>
#include <vector>
#include "block_handler.hpp"

enum class DIRECTIONS { NORTH, SOUTH, EAST, WEST, UP, DOWN, UNDEFINED };

struct raycast_data {
  bool success;
  Vector3i position;
  DIRECTIONS block_face;
};

glm::vec3 face_to_offset(DIRECTIONS face);
DIRECTIONS offset_to_face(glm::vec3 offset);

Vector3 normalize(Vector3 vector);

bool block_face_covered(WorldManager *world, glm::vec3 position,
                        DIRECTIONS face);

// IF THE RAY EVER HAS TARGET ERRORS, ITS BECAUSE THE CONVERSION FROM RAY
// POSITION TO BLOCK POSITION IS WRONG
raycast_data cast_ray(WorldManager *world, Vector3 position, Vector3 direction);

void chunk_data(chunk target_chunk, ObjData& output, WorldManager* world);
void append_chunk_data(ObjData& mesh, ObjData data);
