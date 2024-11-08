#pragma once
#include "../external/glad/include/glad/glad.h"
#include "../external/glfw/include/GLFW/glfw3.h"
#include <string>

class Window {
public:
    Window(int width = 800, int height = 600, const std::string& title = "3D Tree");
    ~Window();

    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    bool init();
    GLFWwindow* getHandle() { return window; }

    float getAspectRatio() const { return static_cast<float>(width) / static_cast<float>(height); }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void setUserPointer(void* ptr) {
        if (window) {
            glfwSetWindowUserPointer(window, ptr);
        }
    }

    template<typename T>
    T* getUserPointer() const {
        if (!window) return nullptr;
        return static_cast<T*>(glfwGetWindowUserPointer(window));
    }

private:
    GLFWwindow* window;
    int width;
    int height;
    std::string title;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};