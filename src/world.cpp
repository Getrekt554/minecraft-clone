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

  int32_t cx = (x >> 4) << 4;
  int32_t cy = (y >> 4) << 4;
  int32_t cz = (z >> 4) << 4;

  uint64_t packed_position = pack_position(cx, cy, cz);

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

void WorldManager::generate_chunks_at_position(Vector3i position) {
  static FastNoiseLite noise_generator;
  static bool noise_initialized = false;
  if (!noise_initialized) {
    noise_generator.SetSeed(1326789);
    noise_generator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise_generator.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise_generator.SetFractalOctaves(4);
    noise_generator.SetFractalLacunarity(2.0f);
    noise_generator.SetFractalGain(0.55f);
    noise_generator.SetFrequency(0.003f);
    noise_initialized = true;
  }

  uint16_t gen_length = (uint16_t)(2 * render_distance + 1);

  int32_t player_chunk_x = position.x >> 4;
  int32_t player_chunk_y = 0;
  int32_t player_chunk_z = position.z >> 4;

  std::array<BLOCK, 4096> ground_blocks;
  ground_blocks.fill(BLOCK::STONE);
  std::array<BLOCK, 4096> air_blocks;
  air_blocks.fill(BLOCK::AIR);

  for (uint16_t z = 0; z < gen_length; z++) {
    int32_t z_position = (player_chunk_z - render_distance + z) * 16;

    for (uint16_t y = 0; y < gen_length; y++) {
      int32_t y_position = (player_chunk_y - render_distance + y) * 16;
      
      for (uint16_t x = 0; x < gen_length; x++) {
        int32_t x_position = (player_chunk_x - render_distance + x) * 16;

        uint64_t packed_position = pack_position(x_position, y_position, z_position);

        if (current_chunks.find(packed_position) != current_chunks.end()) {
          continue;
        }

        std::array<BLOCK, 4096> chunk_blocks{};
        bool has_blocks = false;

        for (int bz = 0; bz < 16; bz++) {
          float abs_z = z_position + bz;

          for (int bx = 0; bx < 16; bx++) {
            float abs_x = x_position + bx;

            float base_noise = noise_generator.GetNoise(abs_x, abs_z);
            int surfaceY = 64 + (base_noise * 30) + (pow(abs(base_noise), 2.5f) * 120);

            for (int by = 0; by < 16; by++) {
              float abs_y = y_position + by;
              
              int local = bx + (bz * 16) + (by * 256);
              
              if (abs_y < surfaceY) {
                chunk_blocks[local] = BLOCK::STONE;
                has_blocks = true;
              } 
              else if (abs_y == surfaceY) {
                chunk_blocks[local] = BLOCK::GRASS;
                has_blocks = true;
              } 
              else {
                chunk_blocks[local] = BLOCK::AIR;
              }
            }
          }
        }

        if (has_blocks) {
          add_chunk(packed_position, chunk_blocks);
        }
      }
    }
  }
}

void WorldManager::mesh_all_chunks(ObjData& mesh_data) {
  for (auto& [packed_pos, chunk_ptr] : current_chunks) {
    chunk_data(*chunk_ptr, mesh_data, this);
  }
}