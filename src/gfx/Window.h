#ifndef GAMEGL_WINDOW_H
#define GAMEGL_WINDOW_H

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "KeyboardManager.h"
#include "MouseManager.h"
#include "Util.h"

#define TICKS_PER_SECOND (60)
#define VSYNC (1)

class Window {
public:
    GLFWwindow *handle;
    int width, height;
    KeyboardManager keyboardManager;
    MouseManager mouseManager;

    unsigned long long last_second,
            frames, fps, last_frame, frame_delta,
            ticks, tps, tick_remainder;

private:
    // Internal Callbacks
    static void _error_callback(int error, const char *description) {
        fprintf(stderr, "Error %d: %s\n", error, description);
    }
    
    static void _key_callback(GLFWwindow *handle, int key, int scancode, int action, int mods);

    static void _cursor_callback(GLFWwindow *handle, double xp, double yp);

    static void _mouse_callback(GLFWwindow *handle, int button, int action, int mods);

    static void _size_callback(GLFWwindow *handle, int width, int height);
    // End Internal Callbacks

public:
    Window(int width, int height, const std::string& title) {
        this->width = width;
        this->height = height;

        this->last_frame = NOW();
        this->last_second = NOW();

        glfwSetErrorCallback(_error_callback);

        if (!glfwInit()) {
            fprintf(stderr, "%s", "error initializing GLFW\n");
            exit(1);
        }

        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        this->handle = glfwCreateWindow(this->width, this->height, title.c_str(), nullptr, nullptr);
        if (this->handle == nullptr) {
            fprintf(stderr, "%s", "error creating window\n");
            glfwTerminate();
            exit(1);
        }

        glfwMakeContextCurrent(this->handle);

        glfwSetWindowAspectRatio(this->handle, 16, 9);
        glfwSetInputMode(this->handle, GLFW_STICKY_KEYS, GLFW_TRUE);
        glfwSetWindowSizeLimits(this->handle, 640, 360, GLFW_DONT_CARE, GLFW_DONT_CARE);

        // callbacks
        glfwSetKeyCallback(this->handle, _key_callback);
        glfwSetCursorPosCallback(this->handle, _cursor_callback);
        glfwSetMouseButtonCallback(this->handle, _mouse_callback);
        glfwSetFramebufferSizeCallback(this->handle, _size_callback);

        if (!gladLoadGL()) {
            fprintf(stderr, "%s", "error initializing GLAD\n");
            glfwTerminate();
            exit(1);
        }

        glfwGetFramebufferSize(this->handle, &this->width, &this->height);
        glViewport(0, 0, this->width, this->height);

        glfwSwapInterval(VSYNC);
    }

    void Run();

protected:
    virtual void OnLoad() = 0;

    virtual void OnClose() = 0;

    /// Call the base OnUpdateFrame prior to the child implementation
    virtual void OnUpdateFrame();

    /// Call the base OnRenderFrame after the child implementation
    virtual void OnRenderFrame();

    /// Call the base OnGameTick prior to the child implementation
    virtual void OnGameTick();

    virtual void OnResize() = 0;

    virtual void OnKeyPress() = 0;
};

#endif //GAMEGL_WINDOW_H
