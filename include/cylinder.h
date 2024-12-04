#pragma once
#include <glad/glad.h>
#include <vector>
#include "renderer.h"

struct CylinderMesh {
    MeshRenderer::BufferObjects buffers;
    float parentRadius;
    float childRadius;
};

class Cylinder {
public:
    static void create(std::vector<float>& vertices, std::vector<unsigned int>& indices,
        float radius, float height, int segments);
	static void createTapered(std::vector<float>& vertices, std::vector<unsigned int>& indices,
		float bottomR, float topR, float height, int segments);
};