#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    Camera(float aspectRatio = 800.0f / 600.0f);

    void update(float deltaTime);
    void processKeyboard(GLFWwindow* window, float deltaTime);
    void toggleAutoRotate();

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

private:
    void updateCameraVectors();
    void orbit(float deltaYaw, float deltaPitch);
    void translate(const glm::vec3& offset);

    // Camera attributes
    glm::vec3 position{ 0.0f, 1.2f, 4.0f };
    glm::vec3 focusPoint{ 0.0f, 1.2f, 0.0f };
    glm::vec3 up{ 0.0f, 1.0f, 0.0f };
    float radius{ 4.0f };

    // Angles
    float yaw{ -90.0f };
    float pitch{ 0.0f };

    const glm::vec3 default_position{ 0.0f, 1.2f, 4.0f };
    const glm::vec3 default_focusPoint{ 0.0f, 1.2f, 0.0f };
    const  glm::vec3 default_up{ 0.0f, 1.0f, 0.0f };
    const float default_yaw{ -90.0f };
    const float default_pitch{ 0.0f };

    // Camera options
    float rotationSpeed{ 40.0f };
    float moveSpeed{ 5.0f };
    float aspectRatio;
    float fov{ 45.0f };
    bool autoRotating{ true };

    // Constants
    const float MIN_RADIUS{ 1.0f };
    const float MAX_RADIUS{ 20.0f };
    const float MIN_PITCH{ -89.0f };
    const float MAX_PITCH{ 89.0f };
};