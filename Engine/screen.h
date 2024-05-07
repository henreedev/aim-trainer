#ifndef SCREEN_H
#define SCREEN_H


#include <iostream>
#include <memory>
#include <ostream>

class Application; // Forward declaration

class Screen
{
protected:
    std::shared_ptr<Application> m_app;
public:
    Screen() {}
    virtual ~Screen() {}
    virtual void update(double deltaTime){}
    virtual void draw(){}
    virtual void keyEvent(int key, int action){}
    virtual void mousePosEvent(double xpos, double ypos){}
    virtual void mouseButtonEvent(int button, int action){}
    virtual void scrollEvent(double distance){}
    virtual void windowResizeEvent(int width, int height){}
    virtual void framebufferResizeEvent(int width, int height){}
};

#endif // SCREEN_H
