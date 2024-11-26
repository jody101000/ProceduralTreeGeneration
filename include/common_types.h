#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>

struct TreeNode;  // Forward declaration
struct AttractionPoint;  // Forward declaration

// Define all shared types in one place
struct TreeNode {
    glm::vec3 position;
    std::vector<AttractionPoint*> linked_points;

    size_t parent = 0;
    std::vector<size_t> children;
    float radius = 1.0f;
};

struct AttractionPoint {
    glm::vec3 position;
    bool reached = false;;
    size_t linked_node = -1;
};