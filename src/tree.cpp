#include "tree.h"
#include <gtc/matrix_transform.hpp>
#include <stack>
#include <unordered_map>
#include <string>
#include <stack>
#include <unordered_map>
#include <cstdlib>  // For randomization
#include <ctime>    // For seeding randomness
#include <random>

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
    const std::string& axiom, const std::unordered_map<char, std::string>& rules,
    float length, float radius, int depth) {

    // Seed random generator for variability
    //std::srand(static_cast<unsigned int>(std::time(0)));

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

    std::random_device rd;  // Seed generator
    std::mt19937 gen(rd()); // Random number generator (Mersenne Twister)
    std::uniform_real_distribution<float> distrib(0.0, length);

    // Generate a random number

    // Interpret the expanded string
    for (char c : current) {
        float random_number = distrib(gen);
        // length = random_number;
        switch (c) {
        case 'F':
            // Draw forward: Add current transformation and move forward
            branchTransforms.push_back(currentModel);
            currentModel = glm::translate(currentModel, glm::vec3(0.0f, length, 0.0f));
            currentModel = glm::scale(currentModel, glm::vec3(length, length, length));
            break;

        case 'X':
        case 'Y':
            // Generate branches based on 'X' or 'Y'
            branchTransforms.push_back(currentModel);
            currentModel = glm::translate(currentModel, glm::vec3(0.0f, length, 0.0f));
            currentModel = glm::scale(currentModel, glm::vec3(length, length, length));
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

        default:
            // Ignore any other symbols
            break;
        }
    }
}


