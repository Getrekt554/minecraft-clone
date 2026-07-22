#pragma once

#include "../external/glm/glm.hpp"
#include "../external/glm/gtc/matrix_transform.hpp"
#include "../external/glm/gtc/type_ptr.hpp"
#include <vector>
#include "renderer.hpp"

void add_block(glm::vec3 pos, std::vector<float> &vertices,
               std::vector<unsigned int> &indices, unsigned int texture);
