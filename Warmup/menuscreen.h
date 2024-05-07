#ifndef MENUSCREEN_H
#define MENUSCREEN_H


#include "Engine/screen.h"
#include <memory>
#include "Graphics/global.h"

#include <GLFW/glfw3.h>
class MenuScreen : public Screen
{
public:
    MenuScreen();
    void draw() override;
    void keyEvent(int key, int action) override;
    void mousePosEvent(double xpos, double ypos) override;
    void mouseButtonEvent(int button, int action) override;
    void switchToGame();
};

#endif // MENUSCREEN_H
