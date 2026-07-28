#include "utilities.hpp"
#include "world.hpp"

Vector3i operator+(const Vector3i &a, const Vector3i &b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z};
};
Vector3i operator-(const Vector3i &a, const Vector3i &b) {
  return {a.x - b.x, a.y - b.y, a.z - b.z};
};
Vector3i operator*(const Vector3i &a, const int b) {
  return {b * a.x, b * a.y, b * a.z};
};

Vector3 operator+(const Vector3 &a, const Vector3 &b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z};
};
Vector3 operator-(const Vector3 &a, const Vector3 &b) {
  return {a.x - b.x, a.y - b.y, a.z - b.z};
};
Vector3 operator*(const Vector3 &a, const float b) {
  return {a.x * b, a.y * b, a.z * b};
};

bool operator==(const Vector3i &a, const Vector3i &b) {
  return (a.x == b.x && a.y == b.y && a.z == b.z);
};
bool operator==(const Vector3 &a, const Vector3 &b) {
  return (a.x == b.x && a.y == b.y && a.z == b.z);
};
bool operator==(const Vector3i &a, const Vector3 &b) {
  return (a.x == b.x && a.y == b.y && a.z == b.z);
};
bool operator==(const Vector3 &a, const Vector3i &b) {
  return (a.x == b.x && a.y == b.y && a.z == b.z);
};


Vector3i face_to_offset(DIRECTIONS face) {
  static constexpr Vector3i FACE_OFFSETS[]{
      {0, 0, -1}, // NORTH
      {0, 0, 1},  // SOUTH
      {1, 0, 0},  // EAST
      {-1, 0, 0}, // WEST
      {0, 1, 0},  // UP
      {0, -1, 0}  // DOWN
  };
  return FACE_OFFSETS[static_cast<int>(face)];
}

DIRECTIONS offset_to_face(Vector3i offset) {
  if (offset == Vector3i{0, 0, -1}) { // NORTH
    return DIRECTIONS::NORTH;
  } else if (offset == Vector3i{0, 0, 1}) { // SOUTH
    return DIRECTIONS::SOUTH;
  } else if (offset == Vector3i{1, 0, 0}) { // EAST
    return DIRECTIONS::EAST;
  } else if (offset == Vector3i{-1, 0, 0}) { // WEST
    return DIRECTIONS::WEST;
  } else if (offset == Vector3i{0, 1, 0}) { // UP
    return DIRECTIONS::UP;
  } else if (offset == Vector3i{0, -1, 0}) { // DOWN
    return DIRECTIONS::DOWN;
  }
  return DIRECTIONS::UNDEFINED;
}

Vector3 normalize(Vector3 vector) {
  float squared_length =
      vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
  if (squared_length > 0.0f) {
    float length = sqrt(squared_length);
    return {vector.x / length, vector.y / length, vector.z / length};
  }

  return {0.0f, 0.0f, 0.0f};
}

bool block_face_covered(WorldManager *world, Vector3i position,
                        DIRECTIONS face) {
  Vector3i offset = face_to_offset(face);
  BLOCK target_block = world->get_block_from_position(
      position.x + offset.x, position.y + offset.y, position.z + offset.z);
  return target_block != BLOCK::AIR;
}

raycast_data cast_ray(WorldManager *world, Vector3 position,
                      Vector3 direction) {
  Vector3 ray_position = position;
  Vector3 norm_direction = normalize(direction);

  constexpr float STEP_SIZE = 0.5f;

  constexpr float MAX_DISTANCE = 16.0f;
  float distance_traveled = 0.0f;

  Vector3i last_block_position =
      (Vector3i){(int32_t)ray_position.x, (int32_t)ray_position.y,
                 (int32_t)ray_position.z};

  while (distance_traveled < MAX_DISTANCE) {
    Vector3i casted_ray_position =
        (Vector3i){(int32_t)ray_position.x, (int32_t)ray_position.y,
                   (int32_t)ray_position.z};

    BLOCK block_on = world->get_block_from_position(
        ray_position.x, ray_position.y, ray_position.z);

    if (block_on != BLOCK::AIR) {
      raycast_data collision_data;

      collision_data.success = true;
      collision_data.position = casted_ray_position;

      if (casted_ray_position == last_block_position) {
        collision_data.block_face = DIRECTIONS::UNDEFINED;
        return collision_data;
      };

      Vector3i offset_vector = last_block_position - casted_ray_position;
      collision_data.block_face = offset_to_face(offset_vector);

      return collision_data;
    }

    ray_position = ray_position + (norm_direction * STEP_SIZE);
    distance_traveled += STEP_SIZE;

    last_block_position = casted_ray_position;
  }
  return {false, {0, 0, 0}, DIRECTIONS::UNDEFINED};
}

void chunk_data(chunk target_chunk, ObjData& output) {
  Vector3i block_pos;

  unpack_position(target_chunk.pos, block_pos.x, block_pos.y, block_pos.z);

  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 16; y++) {
      for (int z = 0; z < 16; z++) {
        add_block(glm::vec3(block_pos.x + x, block_pos.y + y, block_pos.z + z),
                  output.vertices, output.indices,
                  (uint8_t)target_chunk.blocks[(y << 8) | (z << 4) | x]);
      }
    }
  }
}