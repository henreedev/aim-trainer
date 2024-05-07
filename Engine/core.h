#pragma once

#include <GLFW/glfw3.h>
#include "Engine/application.h"
#include "Graphics/global.h"


class Core
{
private:
    std::shared_ptr<Application> m_app;
public:
    Core(std::shared_ptr<Application> app);
    ~Core();
    void update(double deltaTime);
    void draw();
    void keyEvent(int key, int action);
    void mousePosEvent(double xpos, double ypos);
    void mouseButtonEvent(int button, int action);
    void scrollEvent(double distance);
    void windowResizeEvent(int width, int height);
    void framebufferResizeEvent(int width, int height);
};
