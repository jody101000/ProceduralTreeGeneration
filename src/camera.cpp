#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

Camera::Camera(float aspectRatio, glm::vec3 focusPoint)
    : aspectRatio(aspectRatio)
    , focusPoint(focusPoint){
    position = { focusPoint.x, focusPoint.y, focusPoint.z + radius};
    default_position = position;
    default_focusPoint = focusPoint;
    updateCameraVectors();
}

void Camera::update(float deltaTime) {
    if (autoRotating) {
        orbit(rotationSpeed * deltaTime, 0.0f);
    }
}

void Camera::processMouseScroll(float yoffset) {
    //autoRotating = false;  // Stop auto-rotation when scrolling
    glm::vec3 focus = focusPoint;
    moveAlongViewDirection(yoffset * scrollSpeed);
    focusPoint = focus;
    glm::vec3 pos = position;
    focus.y = 0;
    pos.y = 0;
    radius = glm::length(focus - pos);
}

void Camera::moveAlongViewDirection(float amount) {
    glm::vec3 forward = glm::normalize(focusPoint - position);
    float distance = glm::length(focusPoint - position);
    float scaledMove = amount * distance * 0.5f;
    translate(forward * scaledMove);
}

void Camera::processKeyboard(GLFWwindow* window, float deltaTime) {
    float moveAmount = moveSpeed * deltaTime;
    float orbitAmount = rotationSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        autoRotating = false;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        position = default_position;
        focusPoint = default_focusPoint;
        up = default_up;
        yaw = default_yaw;
        pitch = default_pitch;
        autoRotating = true;
    }

    // Orbital rotation
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        orbit(orbitAmount, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        orbit(-orbitAmount, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        orbit(0.0f, orbitAmount);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        orbit(0.0f, -orbitAmount);
    }

    // Forward/backward movement
    glm::vec3 forward = glm::normalize(focusPoint - position);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        moveAlongViewDirection(moveAmount);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        moveAlongViewDirection(-moveAmount);
    }

    // Translation
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
        translate(up * moveAmount);
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
        translate(-up * moveAmount);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        translate(up * moveAmount);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        translate(-up * moveAmount);
    }

    glm::vec3 right = glm::normalize(glm::cross(forward, up));
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        translate(right * moveAmount);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        translate(-right * moveAmount);
    }
}

template<typename T>
T clamp(T value, T min, T max) {
    return value < min ? min : (value > max ? max : value);
}

void Camera::orbit(float deltaYaw, float deltaPitch) {
    yaw += deltaYaw;
    pitch += deltaPitch;
    pitch = clamp(pitch, MIN_PITCH, MAX_PITCH);

    updateCameraVectors();
}

void Camera::translate(const glm::vec3& offset) {
    position += offset;
    focusPoint += offset;
}

void Camera::updateCameraVectors() {
    float x = radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    float y = radius * sin(glm::radians(pitch));
    float z = radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));

    position = focusPoint + glm::vec3(x, y, z);
}

void Camera::toggleAutoRotate() {
    autoRotating = !autoRotating;
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, focusPoint, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
}