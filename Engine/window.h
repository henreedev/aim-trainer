#pragma once

#include "Engine/application.h"
#include "core.h"

class Window
{
public:
    Window(GLFWwindow* wins);
    ~Window();

    int start(int appId);
    int loop();

private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void windowSizeCallback(GLFWwindow* window, int width, int height);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* m_GLFWwindow;
    bool m_windowAllocated = false;
    bool m_glfwInitialized = false;
    Core* m_core;
    bool m_coreAllocated = false;
    const double m_secPerUpdate = 1.0/60;
    double previous;
};
