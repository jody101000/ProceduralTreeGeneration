#include "renderer.h"

MeshRenderer::BufferObjects MeshRenderer::createBuffers(
    const std::vector<float>& vertices,
    const std::vector<unsigned int>& indices) {

    BufferObjects buffers;
    buffers.indexCount = indices.size();

    // Generate and bind buffers
    glGenVertexArrays(1, &buffers.VAO);
    glGenBuffers(1, &buffers.VBO);
    glGenBuffers(1, &buffers.EBO);

    glBindVertexArray(buffers.VAO);

    // Buffer vertex data
    glBindBuffer(GL_ARRAY_BUFFER, buffers.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
        vertices.data(), GL_STATIC_DRAW);

    // Buffer index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        indices.data(), GL_STATIC_DRAW);

    // Set vertex attributes
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
        (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return buffers;
}

void MeshRenderer::deleteBuffers(BufferObjects& buffers) {
    if (buffers.VAO != 0) {
        glDeleteVertexArrays(1, &buffers.VAO);
        glDeleteBuffers(1, &buffers.VBO);
        glDeleteBuffers(1, &buffers.EBO);
        buffers.VAO = buffers.VBO = buffers.EBO = 0;
        buffers.indexCount = 0;
    }
}