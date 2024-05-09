#include "menuscreen.h"

#include "Graphics/global.h"

MenuScreen::MenuScreen() {}


void MenuScreen::draw() {
    Global::graphics.setClearColor(glm::vec3(0.1f, 0.1f, 0.1f));
    Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Global::graphics.bindShader("text");
    Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "Paused (click anywhere to play)",
                                glm::ivec2(Global::graphics.getFramebufferSize().x / 2, Global::graphics.getFramebufferSize().y / 2),
                                AnchorPoint::TopCenter,
                                Global::graphics.getFramebufferSize().x,
                                0.8f, 0.1f, glm::vec3(.8, .8, 1));
}
void MenuScreen::keyEvent(int key, int action) {
    if(key == GLFW_KEY_ESCAPE){}
}
void MenuScreen::mousePosEvent(double xpos, double ypos) {}
void MenuScreen::mouseButtonEvent(int button, int action) {}
void MenuScreen::switchToGame() {}