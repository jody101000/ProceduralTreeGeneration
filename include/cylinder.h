#pragma once
#include <vector>

class Cylinder {
public:
    static void create(std::vector<float>& vertices, std::vector<unsigned int>& indices,
        float radius, float height, int segments);
};