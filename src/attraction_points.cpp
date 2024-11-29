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
    //std::random_device rd;
    //std::mt19937 gen(rd());
    //std::uniform_real_distribution<float> dist(-0.2f, 0.2f);

    // deterministic random
    const unsigned int FIXED_SEED = 120;
    std::mt19937 gen(FIXED_SEED);
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
    for (auto& node : tree_node_manager.tree_nodes) {
        node.linked_points.clear();
    }

    for (auto& point : attraction_points) {
        if (point.reached) continue;

        point.linked_node = -1;

        float closest_distance = std::numeric_limits<float>::max();
        size_t closest_node = -1;

        for (size_t i = 0; i < tree_node_manager.tree_nodes.size(); i++) {
            TreeNode node = tree_node_manager.tree_nodes[i];
            float distance = glm::length(point.position - node.position);

            // Check if node is within influence radius and closer than previous matches
            if (distance <= influence_radius && distance < closest_distance) {
                closest_distance = distance;
                closest_node = i;
            }
            // Attraction point reached
            if (distance <= min_distance) {
                point.reached = true;
            }
        }
        // If we found a valid node within radius, establish the bidirectional link
        if (closest_node != -1) {
            point.linked_node = closest_node;
            tree_node_manager.tree_nodes[closest_node].linked_points.push_back(&point);
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