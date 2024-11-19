#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "shader.h"
#include "cylinder.h"
#include "tree.h"
#include "camera.h"
#include "window.h"
#include "sphere.h"
#include "attraction_points.h"
#include "renderer.h"
#include <vector>
#include <iostream> 
#include <memory> 
#define SHADER_PATH(name) SHADER_DIR name

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
    Shader shader(SHADER_PATH("vertex_shader.glsl"),
                  SHADER_PATH("fragment_shader.glsl"));

    // Generate cylinder mesh
    std::vector<float> cylinderVertices;
    std::vector<unsigned int> cylinderIndices;
    Cylinder::create(cylinderVertices, cylinderIndices, 0.1f, 1.0f, 8);

    // Create cylinder buffers
    auto cylinderBuffers = MeshRenderer::createBuffers(cylinderVertices, cylinderIndices);

    // Generate sphere mesh for attraction points
    std::vector<float> sphereVertices;
    std::vector<unsigned int> sphereIndices;
    Sphere::create(sphereVertices, sphereIndices, 0.05f, 12, 12);

    // Create sphere buffers
    auto sphereBuffers = MeshRenderer::createBuffers(sphereVertices, sphereIndices);

    // Create Atrtaction Points
    Envelope envelope;
    envelope.position = glm::vec3(0.0f, 0.0f, 0.0f);
    envelope.dimension = glm::vec3(1.0f, 1.0f, 1.0f);
    envelope.num_points[0] = 4;
    envelope.num_points[1] = 4;
    envelope.num_points[2] = 4;
    AttractionPointManager attractionPoints(envelope);

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
    glm::vec3 pointColor(1.0f, 0.0f, 0.0f);

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

        // Draw tree branches
        glBindVertexArray(cylinderBuffers.VAO);
        shader.setVec3("objectColor", treeColor);
        for (const auto& transform : branchTransforms) {
            shader.setMat4("model", transform);
            glDrawElements(GL_TRIANGLES, cylinderBuffers.indexCount, GL_UNSIGNED_INT, 0);
        }

        // Draw attraction points
        glBindVertexArray(sphereBuffers.VAO);
        shader.setVec3("objectColor", pointColor);
        for (const auto& point : attractionPoints.attraction_points) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, point.position);
            shader.setMat4("model", model);
            glDrawElements(GL_TRIANGLES, sphereBuffers.indexCount, GL_UNSIGNED_INT, 0);
        }

        // close the window when esc is clicked
        if (glfwGetKey(window.getHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window.getHandle(), true);
        }

        window.swapBuffers();
        window.pollEvents();
    }

    // Cleanup
    MeshRenderer::deleteBuffers(cylinderBuffers);
    MeshRenderer::deleteBuffers(sphereBuffers);

    // Camera will be automatically cleaned up when unique_ptr goes out of scope
    g_camera = nullptr;

    return 0;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (g_camera) {
        g_camera->processMouseScroll(static_cast<float>(yoffset));
    }
}