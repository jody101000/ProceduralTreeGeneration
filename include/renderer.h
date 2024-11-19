#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class MeshRenderer {
public:
    struct BufferObjects {
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        size_t indexCount;

        BufferObjects() : VAO(0), VBO(0), EBO(0), indexCount(0) {}
    };

    static BufferObjects createBuffers(const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices);

    static void deleteBuffers(BufferObjects& buffers);
};