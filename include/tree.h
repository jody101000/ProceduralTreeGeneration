#pragma once
#include <vector>
#include <glm.hpp>
#include <string>
#include <unordered_map>
#include "tree.h"
#include "tree_nodes.h"
#include "common_types.h"
#include "cylinder.h"

class Tree {
public:
    static void createBranches(glm::mat4& model, std::vector<glm::mat4>& branchTransforms,
        float length, float radius, int depth);

    static void createBranchesLSystem(glm::mat4& model, std::vector<glm::mat4>& branchTransforms,
        std::vector<glm::mat4>& leafTransforms, const std::string& axiom,
        const std::unordered_map<char, std::string>& rules,
        float length, float radius, int depth);

    static void createBranchesSpaceColonization(std::vector<TreeNode>& tree_nodes, glm::mat4& model, std::vector<glm::mat4>& branchTransforms,
        float length, float radius, int depth, int root_nodes);
};