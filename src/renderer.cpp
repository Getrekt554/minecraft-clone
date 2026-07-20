#include "renderer.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"
#include <vector>

std::vector<float> vertices = {
    // Positions          // Colors           // Texture Coords
    // Back Face
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f, 1.0f,
    1.0f, 1.0f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

    // Front Face
    -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.5f, 1.0f,
    1.0f, 1.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

    // Left Face
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -0.5f, 0.5f, -0.5f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

    // Right Face
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f, -0.5f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

    // Bottom Face
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.5f, -0.5f, -0.5f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

    // Top Face
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.5f, 0.5f, -0.5f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f};

std::vector<unsigned int> indices = {
    0,  1,  2,  0,  2,  3,  // Back
    4,  5,  6,  4,  6,  7,  // Front
    8,  9,  10, 8,  10, 11, // Left
    12, 13, 14, 12, 14, 15, // Right
    16, 17, 18, 16, 18, 19, // Bottom
    20, 21, 22, 20, 22, 23  // Top
};
const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "   TexCoord = aTexCoord;\n"
    "}\0";

const char *fragShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D ourTexture;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);\n"
    "}\0";

void Renderer::init() {
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (8 * sizeof(float)),
                        (void *)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (8 * sizeof(float)),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // UV attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (8 * sizeof(float)),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);

  {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../textures/blocks/grass.png", &width,
                                    &height, &nrChannels, 0);

    if (!data) {
      std::cerr << "check filepath retep\n";
      stbi_image_free(data);
      exit(-1);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
  }

  // shader
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertex_shader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(frag_shader, 1, &fragShaderSource, NULL);
  glCompileShader(frag_shader);

  glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(frag_shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  shader_program = glCreateProgram();

  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, frag_shader);
  glLinkProgram(shader_program);

  // delete shaders
  glDeleteShader(vertex_shader);
  glDeleteShader(frag_shader);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::tick(Camera camera) {
  glClearColor(0.00f, 0.60f, 0.70f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(shader_program);

  camera.tick(shader_program);
}

void Renderer::draw() {
  glBindTexture(GL_TEXTURE_2D, texture);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
