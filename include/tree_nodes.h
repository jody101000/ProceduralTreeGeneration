#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "common_types.h"


class TreeNodeManager {
public:
	/* constructor */
	TreeNodeManager(int initial_num);

	bool GrowNewNodes(float growth_distance);
	void DebugPrintNodes();
	std::vector<TreeNode> tree_nodes;
private:
	void InitializeTreeNodes(int initial_num);
	glm::vec3 GrowthDirection(TreeNode& node);
};
