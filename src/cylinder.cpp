#include "cylinder.h"
#include <cmath>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
void Cylinder::createTapered(std::vector<float>& vertices, std::vector<unsigned int>& indices,
    float bottomR, float topR, float height, int segments) {
    float segmentAngle = 2.0f * M_PI / segments;

    for (int i = 0; i <= segments; ++i) {
        float angle = i * segmentAngle;
        float xBottom = bottomR * cos(angle);
        float zBottom = bottomR * sin(angle);

        vertices.push_back(xBottom);
        vertices.push_back(0.0f);
        vertices.push_back(zBottom);
        vertices.push_back(xBottom / bottomR);
        vertices.push_back(0.0f);
        vertices.push_back(zBottom / bottomR);

        float xTop = topR * cos(angle);
        float zTop = topR * sin(angle);

        vertices.push_back(xTop);
        vertices.push_back(height);
        vertices.push_back(zTop);
        vertices.push_back(xTop / topR);
        vertices.push_back(0.0f);
        vertices.push_back(zTop / topR);
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
