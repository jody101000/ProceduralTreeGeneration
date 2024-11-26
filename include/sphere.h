// sphere.h
#pragma once
#include <vector>
#define M_PI 3.14159265358979323846

class Sphere {
public:
    static void create(std::vector<float>& vertices, std::vector<unsigned int>& indices,
        float radius, int sectors, int stacks) {
        vertices.clear();
        indices.clear();

        float sectorStep = 2 * M_PI / sectors;
        float stackStep = M_PI / stacks;

        // Generate vertices
        for (int i = 0; i <= stacks; ++i) {
            float stackAngle = M_PI / 2 - i * stackStep;
            float xy = radius * cosf(stackAngle);
            float z = radius * sinf(stackAngle);

            for (int j = 0; j <= sectors; ++j) {
                float sectorAngle = j * sectorStep;

                // Vertex position
                float x = xy * cosf(sectorAngle);
                float y = xy * sinf(sectorAngle);

                // Normalized vertex normal
                float nx = x / radius;
                float ny = y / radius;
                float nz = z / radius;

                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
                vertices.push_back(nx);
                vertices.push_back(ny);
                vertices.push_back(nz);
            }
        }

        // Generate indices
        for (int i = 0; i < stacks; ++i) {
            int k1 = i * (sectors + 1);
            int k2 = k1 + sectors + 1;

            for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
                if (i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                if (i != (stacks - 1)) {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }
    }
};