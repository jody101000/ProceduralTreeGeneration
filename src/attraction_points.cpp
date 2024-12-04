#include "attraction_points.h"
#include "tree_nodes.h"
#include "common_types.h"
#include <iostream>
#include <random>

AttractionPointManager::AttractionPointManager(Envelope envelope) {
    this->envelope = envelope;
    CreatePoints();
}


void AttractionPointManager::EvenlyDistribute() {
    //// Random number generator setup, real random
    std::random_device rd;
    std::mt19937 gen(rd());
    //std::uniform_real_distribution<float> dist(-0.2f, 0.2f);

    // deterministic random
    // const unsigned int FIXED_SEED = 120;
    // std::mt19937 gen(FIXED_SEED);
    std::uniform_real_distribution<float> dist(-0.2f, 0.2f);

    for (int x = -envelope.negative_x; x <= envelope.positive_x; x++) {
        for (int y = 0; y <= envelope.positive_y; y++) {
            for (int z = -envelope.negative_z; z <= envelope.positive_z; z++) {
                AttractionPoint point;

                // Calculate base position
                glm::vec3 basePosition(
                    envelope.position.x + envelope.interval.x * x,
                    envelope.position.y + envelope.interval.y * y,
                    envelope.position.z + envelope.interval.z * z
                );

                // Apply density factor to the random offset
                glm::vec3 randomOffset(
                    dist(gen),
                    dist(gen),
                    dist(gen)
                );

                // Final position combines base position with scaled random offset
                point.position = basePosition + randomOffset;

                point.reached = false;
                attraction_points.push_back(point);
            }
        }
    }
}

void AttractionPointManager::CreatePoints() {
    if (true) {// do even distribution 
        EvenlyDistribute();
    }
}

void AttractionPointManager::UpdateLinks(TreeNodeManager& tree_node_manager, const float influence_radius, const float min_distance) {
    const float influence_radius_sq = influence_radius * influence_radius;
    const float min_distance_sq = min_distance * min_distance;
    
    for (auto& node : tree_node_manager.tree_nodes) {
        node.linked_points.clear();
    }

    struct GridCell {
        std::vector<size_t> node_indices;
    };

    const float cell_size = influence_radius;
    std::unordered_map<size_t, GridCell> grid;

    for (size_t i = 0; i < tree_node_manager.tree_nodes.size(); i++) {
        const auto& node = tree_node_manager.tree_nodes[i];
        size_t cell_x = static_cast<size_t>(node.position.x / cell_size);
        size_t cell_y = static_cast<size_t>(node.position.y / cell_size);
        size_t cell_z = static_cast<size_t>(node.position.z / cell_size);
        size_t cell_key = (cell_x << 20) | (cell_y << 10) | cell_z;
        grid[cell_key].node_indices.push_back(i);
    }

    #pragma omp parallel for if(attraction_points.size() > 1000)
    for (size_t p = 0; p < attraction_points.size(); p++) {
        auto& point = attraction_points[p];
        if (point.reached) continue;

        point.linked_node = -1;
        float closest_distance_sq = std::numeric_limits<float>::max();
        size_t closest_node = -1;

        // Check only neighboring cells
        size_t cell_x = static_cast<size_t>(point.position.x / cell_size);
        size_t cell_y = static_cast<size_t>(point.position.y / cell_size);
        size_t cell_z = static_cast<size_t>(point.position.z / cell_size);

        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                for (int dz = -1; dz <= 1; dz++) {
                    size_t cell_key = ((cell_x + dx) << 20) | ((cell_y + dy) << 10) | (cell_z + dz);
                    auto it = grid.find(cell_key);
                    if (it == grid.end()) continue;

                    // Check nodes in this cell
                    for (size_t node_idx : it->second.node_indices) {
                        const auto& node = tree_node_manager.tree_nodes[node_idx];
                        const glm::vec3 diff = point.position - node.position;
                        const float distance_sq = glm::dot(diff, diff);

                        if (distance_sq <= influence_radius_sq && distance_sq < closest_distance_sq) {
                            closest_distance_sq = distance_sq;
                            closest_node = node_idx;
                        }
                        if (distance_sq <= min_distance_sq) {
                            point.reached = true;
                            break;
                        }
                    }
                }
            }
        }

        if (closest_node != -1) {
            point.linked_node = closest_node;
#pragma omp critical
            {
                tree_node_manager.tree_nodes[closest_node].linked_points.push_back(&point);
            }
        }
    }

}

int AttractionPointManager::GetAvailablePointNumber() {
    int num = 0;
    for (auto& point : attraction_points) {
        if (!point.reached) num++;
    }
    return num;
}

void AttractionPointManager::DebugPrintPoints(TreeNodeManager& tree_node_manager) {
    int i = 0;
    for (auto& point : attraction_points) {
        printf("Attraction Point [%d] (%f, %f, %f), [%s]\n", i, point.position.x, point.position.y, point.position.z, point.reached ? "Reached" : "UnReached");
        printf("\tLinked to Node: ");
        if (point.linked_node == -1) {
            printf("N/A\n\n");
        }
        else {
            printf("(%f, %f, %f)\n\n", tree_node_manager.tree_nodes[point.linked_node].position.x, 
                tree_node_manager.tree_nodes[point.linked_node].position.y, 
                tree_node_manager.tree_nodes[point.linked_node].position.z);
        }
        i++;
    }
}