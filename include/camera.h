#pragma once
#include <glm.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 2.0f, 5.0f),
        glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

    void updateRotation(float deltaTime);
    void processKeyboard(GLFWwindow* window, float deltaTime);
    void processMouseScroll(float yoffset);

    void setTarget(const glm::vec3& newTarget) {
        target = newTarget;
        updatePosition();  // Update camera position relative to new target
    }

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;

    const glm::vec3 getPosition() const { return position; }
    const glm::vec3& getTarget() const { return target; }

private:
    const glm::vec3 defaultPosition{ 0.0f, 2.0f, 5.0f };
    const glm::vec3 defaultTarget{ 0.0f, 0.0f, 0.0f };
    const float defaultHeight = 2.0f;
    const float defaultRadius = 5.0f;
    const float defaultAngle = 0.0f;

    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 front;

    // Camera parameters
    float radius = defaultRadius;
    float rotationSpeed = 0.5f;
    float angle = defaultAngle;

    // Camera options
    float zoom = 45.0f;
    float zoomSpeed = 2.0f;
    float minZoom = 1.0f;
    float maxZoom = 45.0f;

    // Movement options
    float moveSpeed = 2.0f;
    bool autoRotate = true;
    bool hasMovedFromDefault = false;

    void updatePosition();
    void updateVectors();
    void disableAutoRotation();
};
