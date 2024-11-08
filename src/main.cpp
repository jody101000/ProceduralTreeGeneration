#include "../external/glad/include/glad/glad.h"
#include "../external/glfw/include/GLFW/glfw3.h"
#include "../external/glm/glm.hpp"
#include "../external/glm/gtc/matrix_transform.hpp"
#include "../external/glm/gtc/type_ptr.hpp"
#include "../include/shader.h"
#include "../include/cylinder.h"
#include "../include/tree.h"
#include "../include/camera.h"
#include "../include/window.h"
#include <vector>
#include <iostream> 
#include <memory> 

Camera* g_camera = nullptr;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main() {
    // Create and initialize window
    Window window(800, 600, "3D Tree");
    if (!window.init()) {
        return -1;
    }

    // Set up callbacks
    glfwSetScrollCallback(window.getHandle(), scroll_callback);

    // Create shader
    Shader shader("resource/shaders/vertex_shader.glsl", "resource/shaders/fragment_shader.glsl");

    // Generate cylinder mesh
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    Cylinder::create(vertices, indices, 0.1f, 1.0f, 8);

    // Create and bind buffers
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Set up vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Generate branch transforms
    std::vector<glm::mat4> branchTransforms;
    glm::vec3 treePosition(0.0f, -1.0f, 0.0f); // Example: moves tree to x=-2, z=1

    // Replace the existing model matrix creation with:
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, treePosition);
    Tree::createBranches(model, branchTransforms, 1.0f, 0.1f, 4);

    // Light settings
    glm::vec3 lightPos(2.0f, 5.0f, 2.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 treeColor(0.45f, 0.32f, 0.12f);

    // Create camera and set global pointer
    auto camera = std::make_unique<Camera>();
    g_camera = camera.get();

    // For calculating delta time
    float lastFrame = 0.0f;

    // Render loop
    while (!window.shouldClose()) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.8f, 0.9f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        // Update camera
        camera->processKeyboard(window.getHandle(), deltaTime);
        camera->updateRotation(deltaTime);

        // Get updated matrices
        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 projection = camera->getProjectionMatrix(window.getAspectRatio());

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("lightColor", lightColor);
        shader.setVec3("objectColor", treeColor);

        // Draw tree
        for (const auto& transform : branchTransforms) {
            shader.setMat4("model", transform);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        }

        window.swapBuffers();
        window.pollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // Camera will be automatically cleaned up when unique_ptr goes out of scope
    g_camera = nullptr;

    return 0;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (g_camera) {
        g_camera->processMouseScroll(static_cast<float>(yoffset));
    }
}