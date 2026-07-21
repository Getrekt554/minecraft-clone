#pragma once

#include "world.hpp"
#include <cmath>

struct Vector3i {
    int32_t x, y, z;
};

struct Vector3 {
    float x, y, z;
};

Vector3i operator+(const Vector3i& a, const Vector3i& b) {return {a.x + b.x, a.y + b.y, a.z + b.z};};
Vector3i operator-(const Vector3i& a, const Vector3i& b) {return {a.x - b.x, a.y - b.y, a.z - b.z};};
Vector3i operator*(const Vector3i& a, const float b) {return {a.x * b, a.y * b, a.z * b};};

Vector3 operator+(const Vector3& a, const Vector3& b) {return {a.x + b.x, a.y + b.y, a.z + b.z};};
Vector3 operator-(const Vector3& a, const Vector3& b) {return {a.x - b.x, a.y - b.y, a.z - b.z};};
Vector3 operator*(const Vector3& a, const float b) {return {a.x * b, a.y * b, a.z * b};};

bool operator==(const Vector3i& a, const Vector3i& b) {return (a.x == b.x && a.y == b.y && a.z == b.z);};
bool operator==(const Vector3& a, const Vector3& b) {return (a.x == b.x && a.y == b.y && a.z == b.z);};
bool operator==(const Vector3i& a, const Vector3& b) {return (a.x == b.x && a.y == b.y && a.z == b.z);};
bool operator==(const Vector3& a, const Vector3i& b) {return (a.x == b.x && a.y == b.y && a.z == b.z);};


enum class DIRECTIONS {
    NORTH,
    SOUTH,
    EAST,
    WEST,
    UP,
    DOWN,
    UNDEFINED
};

struct raycast_data{
    bool success;
    Vector3i position;
    DIRECTIONS block_face;
};

Vector3i face_to_offset(DIRECTIONS face);
DIRECTIONS offset_to_face(Vector3i offset);

Vector3 normalize(Vector3 vector);

bool block_face_covered(world_manager* world, Vector3i position, DIRECTIONS face);

//IF THE RAY EVER HAS TARGET ERRORS, ITS BECAUSE THE CONVERSION FROM RAY POSITION TO BLOCK POSITION IS WRONG
raycast_data cast_ray(world_manager* world, Vector3 position, Vector3 direction);