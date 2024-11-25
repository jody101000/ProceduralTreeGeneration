#pragma once
#include <vector>

// Function to create a simple leaf geometry
// Parameters:
//   vertices - A reference to a vector to store the generated vertices
//   indices - A reference to a vector to store the generated indices
//   width - The width of the leaf
//   height - The height of the leaf
class leaf {
public:
    static void createLeaf(std::vector<float>& vertices, std::vector<unsigned int>& indices);
};
