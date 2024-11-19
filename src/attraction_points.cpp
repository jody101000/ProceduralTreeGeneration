#include "attraction_points.h"

AttractionPointManager::AttractionPointManager(Envelope envelope) {
    this->envelope = envelope;
    InitializePoints();
}

void AttractionPointManager::EvenlyDistribute() {
    int num_x = envelope.num_points[0];
    int num_y = envelope.num_points[1];
    int num_z = envelope.num_points[2];

    float step_x = envelope.dimension.x * 2 / (num_x - 1);
    float step_y = envelope.dimension.y * 2 / (num_y - 1);
    float step_z = envelope.dimension.z * 2 / (num_z - 1);

    for (int x = 0; x < num_x; x++) {
        for (int y = 0; y < num_y; y++) {
            for (int z = 0; z < num_z; z++) {
                AttractionPoint point;
                point.position = glm::vec3(
                    envelope.position.x - envelope.dimension.x + x * step_x,
                    envelope.position.y - envelope.dimension.y + y * step_y,
                    envelope.position.z - envelope.dimension.z + z * step_z
                );
                point.reached = false;
                attraction_points.push_back(point);
            }
        }
    }
}

void AttractionPointManager::InitializePoints() {
    if (true) {// do even distribution 
        EvenlyDistribute();
    }
}