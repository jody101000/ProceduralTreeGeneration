#include "tree_nodes.h"
#include "common_types.h"
#include <random>
#include <glm/glm.hpp>
#include <cmath>
#define M_PI 3.14159265358979323846


TreeNodeManager::TreeNodeManager(int initial_num) {
    InitializeTreeNodes(initial_num);
}

void TreeNodeManager::InitializeTreeNodes(int initial_num) {
    float node_interval = 0.2f;
    float cylinder_radius = 0.1f;

    //// Initialize random number generator
    //std::random_device rd;
    //std::mt19937 gen(rd());
    
    // deterministic random
    const unsigned int FIXED_SEED = 123;
    std::mt19937 gen(FIXED_SEED);

    // Distribution for radius (0 to cylinder_radius)
    std::uniform_real_distribution<float> radius_dist(0.0f, cylinder_radius);
    // Distribution for angle (0 to 2¦Ð)
    std::uniform_real_distribution<float> angle_dist(0.0f, 2.0f * M_PI);

    for (int i = 0; i < initial_num; i++) {
        TreeNode node;

        // Generate random radius and angle
        float r = radius_dist(gen);
        float theta = angle_dist(gen);

        // Convert polar coordinates to Cartesian coordinates
        float x = r * cos(theta);
        float z = r * sin(theta);

        node.position = { x, node_interval * i, z };
        node.linked_points.clear();

        node.parent = -1;
        node.children.clear();
        node.radius = 1.0f;

        tree_nodes.push_back(node);
    }
}

bool TreeNodeManager::GrowNewNodes(float growth_distance) {
    bool grew = false;
    for (size_t i = 0; i < tree_nodes.size(); i++) {
        TreeNode& tree_node = tree_nodes[i];
        if (tree_node.linked_points.empty()) continue;
        
        glm::vec3 growth_dir = GrowthDirection(tree_node);

        if (growth_dir.y < -0.02f) continue;
        bool child_repeat = false;
        if (glm::length(growth_dir) > 0.001f) {
            glm::vec3 new_pos = tree_node.position + growth_dir * growth_distance;

            // Check if the child has already been created
            for (size_t ci = 0; ci < tree_node.children.size(); ci++) {
                if (new_pos == tree_nodes[ci].position) {
                    child_repeat = true;
                }
            }

            if (child_repeat) continue;

            TreeNode child_node;
            child_node.position = new_pos;
            child_node.parent = i;
            child_node.radius = 0.2 + (tree_node.radius - 0.2) * 0.85f;

            tree_node.children.push_back(tree_nodes.size());
            
            tree_nodes.push_back(child_node);
            grew = true;
        }
    }
    return grew;
}

glm::vec3 TreeNodeManager::GrowthDirection(TreeNode& node) {
    glm::vec3 growth_dir(0.0f);
    for (AttractionPoint* point : node.linked_points) {
        glm::vec3 dir = point->position - node.position;
        float length = glm::length(dir);
        if (length > 0.001f) {
            growth_dir += dir / length; // Normalized direction
        }
    }

    // Normalize the final direction
    float total_length = glm::length(growth_dir);
    if (total_length > 0.001f) {
        growth_dir /= total_length;
    }

    return growth_dir;
}


void TreeNodeManager::DebugPrintNodes() {
    for (auto& node : tree_nodes) {
        printf("Tree Node (%f, %f, %f)\n", node.position.x, node.position.y, node.position.z);
        if (node.parent == -1) {
            printf("\tParent Node: N/A\n");
        }
        else {
            printf("\tParent Node: (%f, %f, %f)\n", tree_nodes[node.parent].position.x, tree_nodes[node.parent].position.y, tree_nodes[node.parent].position.z);
        }

        printf("\tChildren Node: \n");
        if (node.children.empty()) {
            printf("\t\tN/A\n");
        }
        else {
            for (auto child : node.children) {
                printf("\t\t(%f, %f, %f)\n", tree_nodes[child].position.x, tree_nodes[child].position.y, tree_nodes[child].position.z);
            }

        }

        printf("\tLinked to Points: \n");
        if (node.linked_points.empty()) {
            printf("\t\tN/A\n");
        }
        else {
            for (auto& point : node.linked_points) {
                printf("\t\t(%f, %f, %f)\n", point->position.x, point->position.y, point->position.z);
            }
           
        }


    }
}