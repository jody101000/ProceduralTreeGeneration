#include "camera.h"
#include <gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up)
    : position(position), target(target), up(up) {
    updateVectors();
}

void Camera::disableAutoRotation() {
    if (autoRotate) {
        autoRotate = false;
        hasMovedFromDefault = true;
    }
}

void Camera::updateVectors() {
    front = glm::normalize(target - position);
    right = glm::normalize(glm::cross(front, up));
    up = glm::normalize(glm::cross(right, front));
}


void Camera::updateRotation(float deltaTime) {
    if (autoRotate) {
        angle += rotationSpeed * deltaTime;
        updatePosition();
    }
}

void Camera::processKeyboard(GLFWwindow* window, float deltaTime) {
    float moveDist = moveSpeed * deltaTime;
    bool moved = false;

    // Manual rotation with A / D keys
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        angle -= rotationSpeed * deltaTime;
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        angle += rotationSpeed * deltaTime;
        moved = true;
    }

    // Height adjustment with W/S keys
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        //position.y += moveDist;
        target += up * moveDist;
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        //position.y -= moveDist;
        target -= up * moveDist;
        moved = true;
    }

    // Pan camera target
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
        //position.y -= moveDist;
        target += up * moveDist;
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
        //position.y += moveDist;
        target -= up * moveDist;
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        target -= right * moveDist;
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        target += right * moveDist;
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        //position.y -= moveDist;
        target += up * moveDist;
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        //position.y += moveDist;
        target -= up * moveDist;
        moved = true;
    }

    // If any movement key was pressed, disable auto-rotation
    if (moved) {
        disableAutoRotation();
    }

    // Reset camera position with SPACE
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        target = defaultTarget;
        radius = defaultRadius;
        angle = defaultAngle;

        // Re-enable auto-rotation only if we had moved from default position
        if (hasMovedFromDefault) {
            autoRotate = true;
            hasMovedFromDefault = false;
        }
    }

    // Constrain values
    radius = glm::clamp(radius, 2.0f, 10.0f);

    updatePosition();
    updateVectors();
}

void Camera::processMouseScroll(float yoffset) {
    zoom -= yoffset * zoomSpeed;
    zoom = glm::clamp(zoom, minZoom, maxZoom);
    disableAutoRotation();
}

void Camera::updatePosition() {
    position.x = target.x + sin(angle) * radius;
    position.y = target.y;
    position.z = target.z + cos(angle) * radius;
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(zoom), aspectRatio, 0.1f, 100.0f);
}