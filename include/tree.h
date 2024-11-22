#pragma once
#include <vector>
#include <glm.hpp>
#include <string>
#include <unordered_map>

class Tree {
public:
    static void createBranches(glm::mat4& model, std::vector<glm::mat4>& branchTransforms,
        float length, float radius, int depth);

    static void createBranchesLSystem(glm::mat4& model, std::vector<glm::mat4>& branchTransforms,
        const std::string& axiom, const std::unordered_map<char, std::string>& rules,
         float length, float radius, int depth );
};
