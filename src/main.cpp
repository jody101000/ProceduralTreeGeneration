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

    // Generate cylinder mesh, variable name changed to be more specific
    std::vector<float> cylinderVertices;
    std::vector<unsigned int> cylinderIndices;
    Cylinder::create(cylinderVertices, cylinderIndices, 0.075f, 1.0f, 8);

    // Create cylinder buffers
    auto cylinderBuffers = MeshRenderer::createBuffers(cylinderVertices, cylinderIndices);

    // Generate branch transforms
    std::vector<glm::mat4> branchTransforms;
    glm::vec3 treePosition(0.0f, -1.0f, 0.0f); // Example: moves tree to x=-2, z=1

    // Replace the existing model matrix creation with:
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, treePosition);
	//model = glm::scale(model, glm::vec3(1, 2, 1));
    //Tree::createBranches(model, branchTransforms, 0.9f, 0.1f, 4);
    std::string axiom = "Y";
    /*std::unordered_map<char, std::string> rules = {
        {'Y', "X[&Y][^Y]"},
        {'X', "XX"},
    };*/
    //std::unordered_map<char, std::string> rules = {
    //{'X', "F[+X][-X][&X][^X]/FX"},   // `X` rule to create branching in multiple directions
    //{'Y', "F[&Y][^Y]/Y[X]"},         // `Y` rule to vary branch angles for a fuller look
    //{'F', "FF"}                      // `F` produces two segments for longer growth
    //};
    std::unordered_map<char, std::string> rules = {
    {'X', "F[+FX][-FX][&FX][^FX]"},   // `X` generates branches in upward and downward directions with smaller branches
    {'F', "FF"},                     // `F` elongates the trunk for taller growth
    {'Y', "F[^Y][&Y][+Y][-Y]/Y"}     // `Y` adds variability to simulate smaller upward branches
    };
    //std::unordered_map<char, std::string> rules = {
    //{'X', "F[-FX][&X]FX"},            // Drooping branches grow downward
    //{'F', "FF"},                      // Elongate trunk and branches
    //{'Y', "F[+Y][-Y][&Y]"}            // Generate additional smaller downward offshoots
    //};

	Tree::createBranchesLSystem(model, branchTransforms, axiom, rules,0.75f, 1.0f, 3);

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
        glBindVertexArray(cylinderBuffers.VAO);
        shader.setVec3("objectColor", treeColor); // redundant here, but you will need to re-assign color for each buffer
        for (const auto& transform : branchTransforms) {
            shader.setMat4("model", transform);
            glDrawElements(GL_TRIANGLES, cylinderBuffers.indexCount, GL_UNSIGNED_INT, 0);
        }

        window.swapBuffers();
        window.pollEvents();
    }

    // Cleanup
    MeshRenderer::deleteBuffers(cylinderBuffers);

    // Camera will be automatically cleaned up when unique_ptr goes out of scope
    g_camera = nullptr;

    return 0;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (g_camera) {
        g_camera->processMouseScroll(static_cast<float>(yoffset));
    }
}