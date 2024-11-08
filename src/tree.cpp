#include "../include/tree.h"
#include "../external/glm/gtc/matrix_transform.hpp"

void Tree::createBranches(glm::mat4& model, std::vector<glm::mat4>& branchTransforms,
    float length, float radius, int depth) {
    if (depth <= 0) return;

    branchTransforms.push_back(model);

    glm::mat4 rightBranch = model;
    rightBranch = glm::translate(rightBranch, glm::vec3(0.0f, length, 0.0f));
    rightBranch = glm::rotate(rightBranch, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    createBranches(rightBranch, branchTransforms, length * 0.7f, radius * 0.7f, depth - 1);

    glm::mat4 leftBranch = model;
    leftBranch = glm::translate(leftBranch, glm::vec3(0.0f, length, 0.0f));
    leftBranch = glm::rotate(leftBranch, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    createBranches(leftBranch, branchTransforms, length * 0.7f, radius * 0.7f, depth - 1);
}