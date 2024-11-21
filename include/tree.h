#pragma once
#include <vector>
#include <glm.hpp>
#include "tree.h"
#include "tree_nodes.h"
#include "common_types.h"
#include "cylinder.h"

class Tree {
public:
    static void createBranches(glm::mat4& model, std::vector<glm::mat4>& branchTransforms,
        float length, float radius, int depth);
    static void createBranchesSpaceColonization(std::vector<TreeNode>& tree_nodes, TreeNode& parent, glm::mat4& model, std::vector<glm::mat4>& branchTransforms,
        float length, float radius, int depth);
    static void createBranchesRootNodes(std::vector<TreeNode>& tree_nodes, glm::mat4& model, std::vector<glm::mat4>& branchTransforms,
        float length, float radius, int depth, int root_nodes);
};