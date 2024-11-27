#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "shader.h"
#include "cylinder.h"
#include "tree.h"
#include "leaf.h"
#include "camera.h"
#include "window.h"
#include "sphere.h"
#include "attraction_points.h"
#include "renderer.h"
#include "common_types.h"
#include "tree_nodes.h"
#include <vector>
#include <iostream> 
#include <memory> 
#define W_WIDTH 800.0f
#define W_HEIGHT 600.0f

#define SHADER_PATH(name) SHADER_DIR name
#define BRANCH_LENGTH 0.2f
#define ROOT_BRANCH_COUNT (int)7
#define MAX_GROW (int)200

// parameters for space colonization
float env_height = 1.0f;
float env_width = 2.0f;
float env_length = 2.0f;
float env_distance = 1.0f;
int density_x = 5;
int density_y = 4;
int density_z = 5;


enum class Mode {
    LSystem,
    SpaceColonization
};

Mode mode = Mode::SpaceColonization;  // Default mode

Camera* g_camera = nullptr;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main() {
    // Create and initialize window
    Window window(W_WIDTH, W_HEIGHT, "3D Tree");
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
    float branchLength = 1.0f;
	if (mode == Mode::SpaceColonization) {
		branchLength = BRANCH_LENGTH + 0.04f;
	}
    Cylinder::create(cylinderVertices, cylinderIndices, 0.05f, branchLength, 8); // cylinder length

    // Create cylinder buffers
    auto cylinderBuffers = MeshRenderer::createBuffers(cylinderVertices, cylinderIndices);

    // Generate branch transforms
    std::vector<glm::mat4> branchTransforms;
    glm::vec3 treePosition(0.0f, 0.0f, 0.0f); // Example: moves tree to x=-2, z=1

    // Replace the existing model matrix creation with:
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, treePosition);
    std::string axiom = "X";

    std::unordered_map<char, std::string> rules = {
        {'X', "F[//+XXL][+++YXL][-&^FXL][&FXL][\^FXL][--^FXL]"},  
        {'F', "F[/+FL][-FL]"},                         
        {'Y', "F[\+&FYL][/-+F^YL][/&F^Y*L][\^FYL][F++++YL]"},       
        {'L', "L[+L][-L][&L][^L]"}                  
    };

	/*std::unordered_map<char, std::string> rules = {
		{'X', "FF[+XLYL++XL-F[+YLXL]][-XL++F-XL]L"},
		{'F', "X\[FXL/[+XLF]]"},
		{'Y', "\\[+F-XL-F][++YLXL]"}
	};*/
    

   /* std::unordered_map<char, std::string> rules = {
		{'X', "FF+[-F-XF-X][+XLXLL][-XLF[^XLL]][++F&XL]L"},
		{'F', "XX"},
	};*/
    

	std::vector<float> leafVertices;
	std::vector<unsigned int> leafIndices;
	leaf::createLeaf(leafVertices, leafIndices);
	auto leafBuffers = MeshRenderer::createBuffers(leafVertices, leafIndices);
	glm::mat4 leafModel = glm::mat4(1.0f);
	std::vector<glm::mat4> leafTransforms;

	if (mode == Mode::LSystem) {
		Tree::createBranchesLSystem(model, branchTransforms, leafTransforms, axiom, rules, 0.75f, 1.0f, 4);
	}
	else if (mode == Mode::SpaceColonization) {
        // Create Atrtaction Points
        Envelope envelope;
        envelope.position = treePosition + glm::vec3{ 0.1f, env_distance, 0.2f };

        envelope.positive_x = density_x;
		envelope.negative_x = density_x;
		envelope.positive_y = density_y;
		envelope.positive_z = density_z;
		envelope.negative_z = density_z;

        float x_interval = env_length / (2.0f * density_x);
		float y_interval = env_height / density_y;
        float z_interval = env_width / (2.0f * density_z);

        envelope.interval = glm::vec3(x_interval, y_interval, z_interval);

        AttractionPointManager attractionPoints(envelope);

        // Generate tree nodes on the root branch
        TreeNodeManager treeNodeManager(ROOT_BRANCH_COUNT);

        float influenceRadius = std::min(std::min(params.envelope_length, params.envelope_height), params.envelope_width) / 2;
        // First growth
        attractionPoints.UpdateLinks(treeNodeManager, influenceRadius, 0.2f);

        int itr = 0;
        bool grew = true;
        while (grew != false && itr < MAX_GROW) {
            grew = treeNodeManager.GrowNewNodes(BRANCH_LENGTH);
            attractionPoints.UpdateLinks(treeNodeManager, influenceRadius, 0.2f);
            itr++;
            if (itr % 50 == 0) {
                printf("%dth growth done. ", itr);
            }
        }

        Tree::createBranchesSpaceColonization(treeNodeManager.tree_nodes, model, branchTransforms, 0.1f, 4, ROOT_BRANCH_COUNT, leafTransforms);
	}


    // Light settings
    std::vector<glm::vec3> lightPositions = {
        glm::vec3(2.0f, 5.0f, 2.0f),
        glm::vec3(-2.0f, 3.0f, -2.0f)
    };
    std::vector<glm::vec3> lightColors = {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    };
    glm::vec3 treeColor(0.45f, 0.32f, 0.12f);
    glm::vec3 pointColor(1.0f, 0.0f, 0.0f);
    glm::vec3 nodeColor(0.0f, 1.0f, 0.0f);

    glm::vec3 cameraPos = treePosition + glm::vec3{0, 1, 0};
    // Create camera and set global pointer
    auto camera = std::make_unique<Camera>(W_WIDTH / W_HEIGHT, cameraPos);
    g_camera = camera.get();
    glViewport(0, 0, W_WIDTH, W_HEIGHT);

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
        camera->update(deltaTime);

        // Get updated matrices
        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 projection = camera->getProjectionMatrix();

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        for (int i = 0; i < lightPositions.size(); i++) {
            shader.setVec3("lights[" + std::to_string(i) + "].position", lightPositions[i]);
            shader.setVec3("lights[" + std::to_string(i) + "].color", lightColors[i]);
        }
        shader.setInt("numLights", lightPositions.size());
        shader.setVec3("objectColor", treeColor);

        // Draw tree branches
        glBindVertexArray(cylinderBuffers.VAO);
        shader.setVec3("objectColor", treeColor);
        for (const auto& transform : branchTransforms) {
            shader.setMat4("model", transform);
            glDrawElements(GL_TRIANGLES, cylinderBuffers.indexCount, GL_UNSIGNED_INT, 0);
        }

        //Draw Leaves
        glBindVertexArray(leafBuffers.VAO);
        shader.setVec3("objectColor", glm::vec3(0.0f, 1.0f, 0.0f));
        for (const auto& transform : leafTransforms) {
            shader.setMat4("model", transform);
            glDrawElements(GL_TRIANGLES, leafBuffers.indexCount, GL_UNSIGNED_INT, 0);
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

    // Camera will be automatically cleaned up when unique_ptr goes out of scope
    g_camera = nullptr;

    return 0;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (g_camera) {
        g_camera->processMouseScroll(static_cast<float>(yoffset));
    }
}