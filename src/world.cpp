#include "world.hpp"
#include "utilities.hpp"

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
  return (((uint64_t)(x)&XZ_MASK) << 37) | (((uint64_t)(z)&XZ_MASK) << 10) | (((uint64_t)(y)&Y_MASK));
}

void unpack_position(int64_t packed, int32_t &x, int32_t &y, int32_t &z) {
  uint64_t raw_x = (packed >> 37) & XZ_MASK;
  uint64_t raw_z = (packed >> 10) & XZ_MASK;
  uint64_t raw_y = packed & Y_MASK;

  x = (raw_x & 0x4000000) ? (int32_t)(raw_x | ~XZ_MASK) : (int32_t)(raw_x);
  y = (raw_y & 0x200) ? (int32_t)(raw_y | ~Y_MASK) : (int32_t)(raw_y);
  z = (raw_z & 0x4000000) ? (int32_t)(raw_z | ~XZ_MASK) : (int32_t)(raw_z);
}

// chunks are 16x16x16
chunk *WorldManager::get_chunk_from_position(int32_t x, int32_t y, int32_t z) {

  uint64_t packed_position = pack_position((x>>4)<<4, (y>>4)<<4, (z>>4)<<4);

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
    // std::cerr << "Tried to access unloaded block\n";
    return BLOCK::AIR;
  }

  uint8_t chunk_rel_x = x & 15;
  uint8_t chunk_rel_y = y & 15;
  uint8_t chunk_rel_z = z & 15;

  uint16_t index = (chunk_rel_y << 8) | (chunk_rel_z << 4) | chunk_rel_x;

  return target_chunk->blocks[index];
}

void WorldManager::set_block_from_position(int32_t x, int32_t y, int32_t z, BLOCK block_id) {
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

void WorldManager::generate_chunks_at_position(Vector3i position, ObjData& mesh_data) {
  uint16_t gen_length = (uint16_t)(2 * render_distance + 1);

  std::array<BLOCK, 4096> ground_blocks;
  ground_blocks.fill(BLOCK::STONE);
  std::array<BLOCK, 4096> air_blocks;
  air_blocks.fill(BLOCK::AIR);

  for (uint16_t z = 0; z < gen_length; z++) {
    int32_t z_position = ((position.z >> 4) - render_distance) * 16 + (z * 16);
    for (uint16_t y = 0; y < gen_length; y++) {
      int32_t y_position = ((position.y >> 4) - render_distance) * 16 + (y * 16);
      for (uint16_t x = 0; x < gen_length; x++) {
        int32_t x_position = ((position.x >> 4) - render_distance) * 16 + (x * 16);

        uint64_t packed_position = pack_position(x_position, y_position, z_position);

        if (y == 0) add_chunk(packed_position, ground_blocks);
        else add_chunk(packed_position, air_blocks);

      }
    }
  }
  for (uint16_t z = 0; z < gen_length; z++) {
    int32_t z_position = ((position.z >> 4) - render_distance) * 16 + (z * 16);
    for (uint16_t y = 0; y < gen_length; y++) {
      int32_t y_position = ((position.y >> 4) - render_distance) * 16 + (y * 16);
      for (uint16_t x = 0; x < gen_length; x++) {
        int32_t x_position = ((position.x >> 4) - render_distance) * 16 + (x * 16);

        uint64_t packed_position = pack_position(x_position, y_position, z_position);

        chunk_data(*current_chunks.at(packed_position), mesh_data, this);

      }
    }
  }
}
