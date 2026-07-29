#include <cstdint>
#include <vector>

struct ObjData {
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
};

enum class BLOCK : uint8_t { AIR, GRASS = 1, STONE = 2 };

struct Vector3i {
  int32_t x, y, z;
};

struct Vector3 {
  float x, y, z;
};

static const int WIDTH = 1920;
static const int HEIGHT = 1080;

static uint8_t render_distance = 3;