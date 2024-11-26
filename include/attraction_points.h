#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include "tree_nodes.h"
#include "common_types.h"

struct Envelope {
    glm::vec3 position = { 0.0f, 0.0f, 0.0f };  // bottom center
    glm::vec3 interval = { 0.4f, 0.4f, 0.4f }; // x, y, z half length starting from the position
    int positive_x = 3;     // numbber of attraction points in +x direction from bottom center
    int negative_x = 2;     // numbber of attraction points in -x direction from bottom center
    int positive_y = 3;     // numbber of attraction points in +y direction from bottom center
    int positive_z = 3;     // numbber of attraction points in +z direction from bottom center
    int negative_z = 3;     // numbber of attraction points in -z direction from bottom center
};

class AttractionPointManager {
public:
    /* constructor */
    AttractionPointManager(Envelope envelope);

    void UpdateLinks(TreeNodeManager& tree_node_manager, const float influence_radius, const float min_distance);
    int GetAvailablePointNumber();
    void DebugPrintPoints(TreeNodeManager& tree_node_manager);
    std::vector<AttractionPoint> attraction_points;
    Envelope envelope;

private:
    void EvenlyDistribute();
    void CreatePoints();
};