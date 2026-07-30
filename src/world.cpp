#include "world.hpp"
#include "utilities.hpp"

WorldManager::WorldManager() {
  noise_initialized = false;

  empty_chunk.blocks.fill(BLOCK::AIR);
  empty_chunk.pos = 0xFFFFFFFFFFFFFFFF;
}

void WorldManager::add_chunk(int64_t position, std::array<BLOCK, 4096> blocks) {
  if (blocks.empty()) {
    current_chunks.insert_or_assign(position, &empty_chunk);
    return;
  }

  chunk *new_chunk = new chunk();

  new_chunk->pos = position;

  new_chunk->blocks = blocks;

  new_chunk->meshed = false;
  
  current_chunks.insert_or_assign(new_chunk->pos, new_chunk);
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
    return BLOCK::STONE;
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
  bool generated_new_chunks = false;

  static FastNoiseLite noise_generator;
  if (!noise_initialized) {
    noise_generator.SetSeed(1326789);
    noise_generator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise_generator.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise_generator.SetFractalOctaves(4);
    noise_generator.SetFractalLacunarity(2.0f);
    noise_generator.SetFractalGain(0.55f);
    noise_generator.SetFrequency(0.0008f);
    noise_initialized = true;
  }

  uint16_t gen_length = (uint16_t)(2 * render_distance + 1);

  int32_t player_chunk_x = position.x >> 4;
  int32_t player_chunk_y = 0;
  int32_t player_chunk_z = position.z >> 4;

  for (uint16_t z = 0; z < gen_length; z++) {
    int32_t z_position = (player_chunk_z - render_distance + z) << 4;

    for (uint16_t y = 0; y < gen_length; y++) {
      int32_t y_position = (player_chunk_y - render_distance + y) << 4;
      
      for (uint16_t x = 0; x < gen_length; x++) {
        int32_t x_position = (player_chunk_x - render_distance + x) << 4;

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
          generated_new_chunks = true;
        }
        else {
          add_chunk(packed_position, empty_chunk.blocks);
        }
      }
    }
  }

  if (generated_new_chunks) {std::cout << "generated new chunks" << std::endl;}
}

void WorldManager::mesh_all_chunks(Vector3i position) {
  int32_t x,y,z;

  for (auto& [pos, chunk_ptr] : current_chunks) {
    unpack_position(pos, x, y, z);
    if ((abs(position.x - x) >> 4) > render_distance) {
      free_chunk(pos);
      continue;
    }
    if ((abs(position.y - y) >> 4) > render_distance) {
      free_chunk(pos);
      continue;
    }
    if ((abs(position.z - z) >> 4) > render_distance) {
      free_chunk(pos);
      continue;
    }

    if (!chunk_ptr->meshed) {
      chunk_data(*chunk_ptr, chunk_ptr->mesh, this);
      chunk_ptr->meshed = true;
    }

    if (!chunk_ptr->uploaded) {
      chunk_ptr->upload_mesh();
      chunk_ptr->uploaded = true;
    }
  }
}

void chunk::upload_mesh() {
  if (mesh.indices.empty()) return;

  if (VAO == 0) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
  } 

  glBindVertexArray(VAO);

  //make sure to change the draw type after we get done with generation so we can edit chunks while the game is running.
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), mesh.vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (11 * sizeof(float)), (void *)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (11 * sizeof(float)), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // UV attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (11 * sizeof(float)), (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  // Light attribute
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, (11 * sizeof(float)), (void *)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);

  index_count = mesh.indices.size();
  glBindVertexArray(0);
}

void chunk::free_mesh() {
  if (VAO != 0) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    VAO = 0;
    VBO = 0;
    EBO = 0;
    index_count = 0;
  }
}

void chunk::draw() {
  if (VAO == 0 || index_count == 0) return;

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
void WorldManager::free_chunk(int64_t position) {
  current_chunks.at(position)->free_mesh();

  delete current_chunks.at(position);
  current_chunks.erase(position);
}