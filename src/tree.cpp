#include "tree.h"
#include "tree_nodes.h"
#include "common_types.h"
#include "cylinder.h"
#include <glm/glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <stack>
#include <unordered_map>
#include <string>
#include <stack>
#include <unordered_map>
#include <cstdlib>  // For randomization
#include <ctime>    // For seeding randomness
#include <random>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

void Tree::createBranches(glm::mat4& model, std::vector<glm::mat4>& branchTransforms,
    float length, float radius, int depth) {
    if (depth <= 0) return;

    branchTransforms.push_back(model);

    glm::mat4 rightBranch = model;
    rightBranch = glm::translate(rightBranch, glm::vec3(0.0f, length, 0.0f));
    rightBranch = glm::rotate(rightBranch, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	rightBranch = glm::scale(rightBranch, glm::vec3(1, length, 1));
    createBranches(rightBranch, branchTransforms, length * 0.7f, radius * 0.7f, depth - 1);

    glm::mat4 leftBranch = model;
    leftBranch = glm::translate(leftBranch, glm::vec3(0.0f, length, 0.0f));
    leftBranch = glm::rotate(leftBranch, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	leftBranch = glm::scale(leftBranch, glm::vec3(1, length, 1));
    createBranches(leftBranch, branchTransforms, length * 0.7f, radius * 0.7f, depth - 1);
}




void Tree::createBranchesLSystem(glm::mat4& model, std::vector<glm::mat4>& branchTransforms,
    std::vector<glm::mat4>& leafTransforms, const std::string& axiom,
    const std::unordered_map<char, std::string>& rules,
    float length, float radius, int depth) {

    const float angleZ = 20.0f; // For '+' and '-'
    const float angleX = 60.0f; // For '&' and '^'
    const float angleY = 30.0f; // For '/' and '\\'

    // Apply the L-system rules to expand the axiom string
    std::string current = axiom;
    for (int i = 0; i < depth; ++i) {
        std::string next;
        for (char c : current) {
            if (rules.find(c) != rules.end()) {
                next += rules.at(c);  // Apply the rule if it exists
            }
            else {
                next += c;  // Keep the character unchanged if there's no rule
            }
        }
        current = next;
    }

    // Stack to handle branching points
    std::stack<glm::mat4> transformStack;
    glm::mat4 currentModel = model;
    // Interpret the expanded string
	int fCount = 0;
    for (char c : current) {
        std::random_device rd;  // Seed the random number generator
        std::mt19937 gen(rd()); // Mersenne Twister engine
        std::uniform_int_distribution<> dis(0,12); // Uniform distribution between 0 and 20
        std::uniform_int_distribution<> disRotate(-120, 120); // Rotation between 0 and 120 degrees
        std::uniform_real_distribution<> disScale(0.5f, length); // Scale between 0.5 and 1.5
        std::uniform_real_distribution<> disTranslate(-0.4f, 0.4f);
		std::uniform_int_distribution<> disBranch(0,1);
        int num_leaves = dis(gen);
		int gen_branch = disBranch(gen);
        switch (c) {
        case 'F':

            // Draw forward: Add current transformation and move forward
            //if (gen_branch != 0 || fCount < 25)
            //{   
                fCount++;
                branchTransforms.push_back(currentModel);
                currentModel = glm::translate(currentModel, glm::vec3(0.0f, length, 0.0f));
                currentModel = glm::scale(currentModel, glm::vec3(length, length, length));
            //}
            break;

        case 'X':
        case 'Y':
            if (gen_branch != 0) {
            // Generate branches based on 'X' or 'Y'
            branchTransforms.push_back(currentModel);
            currentModel = glm::translate(currentModel, glm::vec3(0.0f, length, 0.0f));
            currentModel = glm::scale(currentModel, glm::vec3(length, length, length));
            }
            break;

        case '+':
            // Roll right around Z-axis
            currentModel = glm::rotate(currentModel, glm::radians(angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
            break;

        case '-':
            // Roll left around Z-axis
            currentModel = glm::rotate(currentModel, glm::radians(-angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
            break;

        case '&':
            // Pitch down around X-axis
            currentModel = glm::rotate(currentModel, glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
            break;

        case '^':
            // Pitch up around X-axis
            currentModel = glm::rotate(currentModel, glm::radians(-angleX), glm::vec3(1.0f, 0.0f, 0.0f));
            break;

        case '/':
            // Yaw right around Y-axis
            currentModel = glm::rotate(currentModel, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
            break;

        case '\\':
            // Yaw left around Y-axis
            currentModel = glm::rotate(currentModel, glm::radians(-angleY), glm::vec3(0.0f, 1.0f, 0.0f));
            break;

        case '[':
            // Save the current transformation matrix to the stack
            transformStack.push(currentModel);
            break;

        case ']':
            // Restore the last saved transformation matrix from the stack
            if (!transformStack.empty()) {
                currentModel = transformStack.top();
                transformStack.pop();
            }
            break;

        case 'L':  // 'L' indicates a leaf point

            for (int i = 0; i < num_leaves; i++) {
                float random_angle = disRotate(gen);
                float scaleFactor = disScale(gen);

                float randomTranslateX = disTranslate(gen);
                float randomTranslateY = disTranslate(gen);
                float randomTranslateZ = disTranslate(gen);
                glm::mat4 leafModel = currentModel;

                leafModel = glm::rotate(leafModel, glm::radians(random_angle), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z
                leafModel = glm::rotate(leafModel, glm::radians(random_angle), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X
                leafModel = glm::rotate(leafModel, glm::radians(random_angle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y

                // Apply scaling to vary leaf sizes
                //leafModel = glm::scale(leafModel, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
                // Apply random translation
                //leafModel = glm::translate(leafModel, glm::vec3(randomTranslateX, randomTranslateY, randomTranslateZ));

                leafTransforms.push_back(leafModel);
            }
            break;
        default:
            // Ignore any other symbols
            break;
        }
    }
}


void spaceColonizationGrow(std::vector<TreeNode>& tree_nodes, TreeNode& parent, glm::mat4& model, std::vector<glm::mat4>& branchTransforms,
    float length, float radius, int depth) {
    if (parent.children.empty()) return;

    for (size_t child_i : parent.children) {
        TreeNode child_node = tree_nodes[child_i];
        glm::mat4 child_branch = model;

        // Calculate direction vector from parent to current node
        glm::vec3 direction = child_node.position - parent.position;
        direction = glm::normalize(direction);
        
        child_branch = glm::translate(child_branch, parent.position);

        // Calculate rotation to align with direction vector
        // Default up vector is (0,1,0)
        if (direction != glm::vec3(0.0f, 1.0f, 0.0f)) {
            glm::vec3 rotationAxis = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction);
            float rotationAngle = acos(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), direction));
            child_branch = glm::rotate(child_branch, rotationAngle, rotationAxis);
        }
        child_branch = glm::scale(child_branch, glm::vec3(parent.radius, 1.0f, parent.radius));

        branchTransforms.push_back(child_branch);

        spaceColonizationGrow(tree_nodes, tree_nodes[child_i], model, branchTransforms, length, radius, depth);
    }
}

void Tree::createBranchesSpaceColonization(std::vector<TreeNode>& tree_nodes, glm::mat4& model, std::vector<glm::mat4>& branchTransforms,
    float length, float radius, int depth, int root_nodes) {
    // branchTransforms.push_back(model);
    for (size_t i = 1; i < root_nodes; i++) {
        glm::mat4 main_branch = model;

        // Calculate direction vector from parent to current node
        glm::vec3 direction = tree_nodes[i].position - tree_nodes[i-1].position;
        direction = glm::normalize(direction);

        main_branch = glm::translate(main_branch, tree_nodes[i - 1].position);

        // Calculate rotation to align with direction vector
        // Default up vector is (0,1,0)
        if (direction != glm::vec3(0.0f, 1.0f, 0.0f)) {
            glm::vec3 rotationAxis = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction);
            float rotationAngle = acos(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), direction));
            main_branch = glm::rotate(main_branch, rotationAngle, rotationAxis);
        }

        branchTransforms.push_back(main_branch);
    }

    for (size_t i = 0; i < root_nodes; i++) {
        spaceColonizationGrow(tree_nodes, tree_nodes[i], model, branchTransforms, length, radius, depth);
    }
}