#include "leaf.h"
#include <vector>

void leaf::createLeaf(std::vector<float>& vertices, std::vector<unsigned int>& indices) {

    vertices = {
        // Position x, y, z              Normal nx, ny, nz
        -0.2f,  0.5f,  0.0f,             0.0f,  0.0f,  1.0f,  // Vertex 0
        0.0f,  0.3f,  0.0f,             0.0f,  0.0f,  1.0f,  // Vertex 1
        0.2f,  0.5f,  0.0f,             0.0f,  0.0f,  1.0f,  // Vertex 2
        -0.2f,  0.9f,  0.0f,             0.0f,  0.0f,  1.0f,  // Vertex 3
        0.2f,  0.9f,  0.0f,             0.0f,  0.0f,  1.0f,  // Vertex 4
        0.0f,  1.1f,  0.0f,             0.0f,  0.0f,  1.0f,  // Vertex 5
        0.01f, 0.0f,  0.0f,             0.0f,  0.0f,  1.0f,  // Vertex 6
        -0.01f, 0.0f,  0.0f,             0.0f,  0.0f,  1.0f,  // Vertex 7
        0.01f,  0.4f,  0.0f,             0.0f,  0.0f,  1.0f,  // Vertex 8
        -0.01f, 0.4f,  0.0f,             0.0f,  0.0f,  1.0f,  // Vertex 9
    };

	indices = {
		0,1,2,
        0,2,4,
        0,3,4,
        3,4,5,
		6,7,8,
		7,8,9

	};
}
