#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "tree_nodes.h"

struct AttractionPoint {
    glm::vec3 position;
    bool reached;
    TreeNode linked_node;
};

struct Envelope {
    glm::vec3 position;
    glm::vec3 dimension; // x, y, z half length starting from the position
    int num_points[3];  // number of points in x, y, z directions
};

class AttractionPointManager {
public:
    AttractionPointManager(Envelope envelope);
    std::vector<AttractionPoint> attraction_points;
    Envelope envelope;

private:
    void EvenlyDistribute();
    void InitializePoints();
};