#pragma once
#include <cstring>
#include <cstdint>
#include <unordered_map>

// chunk positions are 64 bit.
// 27 for x 
// 10 for y 
// 27 for z

const uint64_t XZ_MASK = 0x7FFFFFF;
const uint64_t Y_MASK  = 0x3FF;

enum class BLOCK : uint8_t {
    AIR,
    GRASS,
    STONE
};

struct chunk {
    int64_t pos;

    BLOCK blocks[4096];
    bool dirty = true;
};

class world_manager {
    public:
    //TODO: find out a size to reserve for the unordered map to make it WAY faster
    std::unordered_map<int64_t, chunk*> current_chunks;

    chunk* get_chunk_from_position(int32_t x, int32_t y, int32_t z);
    BLOCK* get_block_from_position(int32_t x, int32_t y, int32_t z);
    void set_block_from_position(int32_t x, int32_t y, int32_t z, BLOCK block_id);
};

int64_t pack_position(int32_t x, int32_t y, int32_t z);
void unpack_position(int64_t packed, int32_t& x, int32_t& y, int32_t& z);

void generate_test_chunk(world_manager& world);
