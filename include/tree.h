#pragma once
#include <vector>
#include "../external/glm/glm.hpp"

class Tree {
public:
    static void createBranches(glm::mat4& model, std::vector<glm::mat4>& branchTransforms,
        float length, float radius, int depth);
};