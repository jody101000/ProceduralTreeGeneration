#include "cylinder.h"
#include <cmath>

#define M_PI 3.14159265358979323846

void Cylinder::create(std::vector<float>& vertices, std::vector<unsigned int>& indices,
    float radius, float height, int segments) {
    float segmentAngle = 2.0f * M_PI / segments;

    for (int i = 0; i <= segments; ++i) {
        float angle = i * segmentAngle;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        vertices.push_back(x);
        vertices.push_back(0.0f);
        vertices.push_back(z);
        vertices.push_back(x / radius);
        vertices.push_back(0.0f);
        vertices.push_back(z / radius);

        vertices.push_back(x);
        vertices.push_back(height);
        vertices.push_back(z);
        vertices.push_back(x / radius);
        vertices.push_back(0.0f);
        vertices.push_back(z / radius);
    }

    for (int i = 0; i < segments; ++i) {
        int bottomLeft = i * 2;
        int bottomRight = (i * 2 + 2) % (segments * 2 + 2);
        int topLeft = i * 2 + 1;
        int topRight = (i * 2 + 3) % (segments * 2 + 2);

        indices.push_back(bottomLeft);
        indices.push_back(bottomRight);
        indices.push_back(topRight);

        indices.push_back(bottomLeft);
        indices.push_back(topRight);
        indices.push_back(topLeft);
    }
}