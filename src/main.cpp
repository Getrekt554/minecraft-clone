#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../external/glm/glm.hpp"
#include "../external/glm/gtc/matrix_transform.hpp"
#include "../external/glm/gtc/type_ptr.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"
#include <iostream>

#define PI 3.14159265359

float vertices[] = {
    // Positions          // Colors           // Texture Coords
    // Back Face
    -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f,

    // Front Face
    -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f,

    // Left Face
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 0.0f,

    // Right Face
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 0.0f,

    // Bottom Face
    -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f,

    // Top Face
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f
};

unsigned int indices[] = {
    0,  1,  2,  0,  2,  3,   // Back
    4,  5,  6,  4,  6,  7,   // Front
    8,  9,  10, 8,  10, 11,  // Left
    12, 13, 14, 12, 14, 15,  // Right
    16, 17, 18, 16, 18, 19,  // Bottom
    20, 21, 22, 20, 22, 23   // Top
};

const char *vertexShaderSource = "#version 330 core\n"
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

const char *fragShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D ourTexture;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);\n"
    "}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    
    float sensitivity = 0.1f;

    float Yaw;
    float Pitch;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f) {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        Front = glm::vec3(0.0f, 0.0f, -1.0f);
        update();
    }

    glm::mat4 get_view_matrix() const {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void move(glm::vec3 direction, float distance) {
        Position += direction * distance;
    }

    void rotate(float xoffset, float yoffset) {
        Yaw   += xoffset;
        Pitch += yoffset;

        if (Pitch > 89.0f)  Pitch = 89.0f;
        if (Pitch < -89.0f) Pitch = -89.0f;

        update();
    }

private:
    void update() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        
        Right = glm::normalize(glm::cross(Front, WorldUp));  
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};

double last_mouse_x = 400.0f;
double last_mouse_y = 400.0f;
bool first_mouse = true;
void process_input(GLFWwindow* window, Camera& camera) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    //camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.move(camera.Front, 0.001f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.move(-camera.Front, 0.001f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.move(-camera.Right, 0.001f);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.move(camera.Right, 0.001f);

    //camera rotation
    double mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    //first mouse sets the mouse_pos at startup
    if (first_mouse) {
        last_mouse_x = mouse_x;
        last_mouse_y = mouse_y;
        first_mouse = false;
    }

    float x_offset = (float)(mouse_x - last_mouse_x);
    float y_offset = (float)(last_mouse_y - mouse_y);

    last_mouse_x = mouse_x;
    last_mouse_y = mouse_y;

    camera.rotate(x_offset * camera.sensitivity, y_offset * camera.sensitivity);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "CloneCraft", NULL, NULL);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (window == NULL) {
        std::cout << "Failed to create window\n";
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 800);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void*)0);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //UV attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int texture;
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        int width, height, nrChannels;
        unsigned char* data = stbi_load("../textures/blocks/grass.png", &width, &height, &nrChannels, 0);


        if (!data) {
            std::cerr << "check filepath retep\n";
            stbi_image_free(data);
            exit(-1);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

    //shader
    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex_shader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned int frag_shader;
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(frag_shader, 1, &fragShaderSource, NULL);
    glCompileShader(frag_shader);

    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(frag_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shader_program;
    shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, frag_shader);
    glLinkProgram(shader_program);


    //delete shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    Camera player_camera(glm::vec3(0.0f, 0.0f, 4.0f));

    while(!glfwWindowShouldClose(window)) {
        
        //input
        process_input(window, player_camera);

        //rendering commands
        glClearColor(0.00f, 0.60f, 0.70f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_program);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(30.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 view = player_camera.get_view_matrix();
        
        //camera configurations
        glm::mat4 projection = glm::perspective(glm::radians(90.0f), 800.0f / 800.0f, 0.1f, 100.0f);
        
        unsigned int modelLoc = glGetUniformLocation(shader_program, "model");
        unsigned int viewLoc  = glGetUniformLocation(shader_program, "view");
        unsigned int projLoc  = glGetUniformLocation(shader_program, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //check events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;

    std::cout << vec.x << vec.y << vec.z << std::endl;

    glfwTerminate();
    return 0;
}