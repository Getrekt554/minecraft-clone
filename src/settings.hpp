#include <cstdint>

enum class BLOCK : uint8_t { AIR, GRASS = 1, STONE = 2 };

struct Vector3i {
  int32_t x, y, z;
};

struct Vector3 {
  float x, y, z;
};
