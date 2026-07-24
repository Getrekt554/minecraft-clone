#include "world.hpp"

// for debug//
void generate_test_chunk(WorldManager &world) {
  chunk *test_chunk = new chunk();

  test_chunk->pos = pack_position(1024, 64, 512);

  test_chunk->dirty = false;

  test_chunk->blocks.fill(BLOCK::GRASS);

  world.current_chunks.insert_or_assign(test_chunk->pos, test_chunk);
}
//
void WorldManager::add_chunk(int64_t position, std::array<BLOCK, 4096> blocks) {
  chunk *new_chunk = new chunk();

  new_chunk->pos = position;

  new_chunk->blocks = blocks;

  current_chunks.insert_or_assign(new_chunk->pos, new_chunk);
}

void WorldManager::free_chunk(int64_t position) {
  delete current_chunks.at(position);
  current_chunks.erase(position);
}

int64_t pack_position(int32_t x, int32_t y, int32_t z) {
  return (((uint64_t)(x)&XZ_MASK) << 37) | (((uint64_t)(y)&XZ_MASK) << 10) |
         (((uint64_t)(z)&Y_MASK));
}

void unpack_position(int64_t packed, int32_t &x, int32_t &y, int32_t &z) {
  uint64_t raw_x = (packed >> 37) & XZ_MASK;
  uint64_t raw_z = (packed >> 10) & XZ_MASK;
  uint64_t raw_y = packed & Y_MASK;

  x = (raw_x & 0x4000000) ? (int32_t)(raw_x | ~XZ_MASK) : (int32_t)(raw_x);
  y = (raw_y & 0x200) ? (int32_t)(raw_y | ~Y_MASK) : (int32_t)(raw_y);
  z = (raw_z & 0x4000000) ? (int32_t)(raw_z | ~XZ_MASK) : (int32_t)(raw_z);
}

Vector3i chunk_relative_pos_to_absolute(int64_t chunk_pos, int32_t x, int32_t y,
                                        int32_t z) {
  Vector3i final;
  unpack_position(chunk_pos, final.x, final.y, final.z);
  final.x += x;
  final.y += y;
  final.z += z;

  return final;
}

// chunks are 16x16x16
chunk *WorldManager::get_chunk_from_position(int32_t x, int32_t y, int32_t z) {
  // im using bitshifting cause im tuff (and its way faster)
  int32_t chunk_x = x >> 4;
  int32_t chunk_y = y >> 4;
  int32_t chunk_z = z >> 4;

  uint64_t packed_position = pack_position(chunk_x, chunk_y, chunk_z);

  auto target = current_chunks.find(packed_position);

  if (target != current_chunks.end()) {
    chunk *target_chunk = target->second;
    return target_chunk;
  }
  return nullptr;
}

BLOCK WorldManager::get_block_from_position(int32_t x, int32_t y, int32_t z) {
  chunk *target_chunk = get_chunk_from_position(x, y, z);

  if (target_chunk == nullptr) {
    std::cerr << "Tried to access unloaded block\n";
    exit(-1);
  }

  uint8_t chunk_rel_x = x & 15;
  uint8_t chunk_rel_y = y & 15;
  uint8_t chunk_rel_z = z & 15;

  uint16_t index = (chunk_rel_y << 8) | (chunk_rel_z << 4) | chunk_rel_x;

  return target_chunk->blocks[index];
}

void WorldManager::set_block_from_position(int32_t x, int32_t y, int32_t z,
                                           BLOCK block_id) {
  chunk *target_chunk = get_chunk_from_position(x, y, z);

  if (target_chunk == nullptr) {
    return;
  }

  uint8_t chunk_rel_x = x & 15;
  uint8_t chunk_rel_y = y & 15;
  uint8_t chunk_rel_z = z & 15;

  uint16_t index = (chunk_rel_y << 8) | (chunk_rel_z << 4) | chunk_rel_x;

  target_chunk->dirty = true;
  target_chunk->blocks[index] = block_id;
}
