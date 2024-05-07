#include "Engine/application.h"
#include "core.h"

Core::Core(std::shared_ptr<Application> app) : m_app(app)
{

}

Core::~Core(){

}

void Core::update(double deltaTime){
    m_app->update(deltaTime);
    Global::input.update(deltaTime);
}

void Core::draw(){
    m_app->draw();
}

void Core::keyEvent(int key, int action){
    Global::input.keyEvent(key, action);
}

void Core::mousePosEvent(double xpos, double ypos){
    Global::input.mousePosEvent(xpos, ypos);
}

void Core::mouseButtonEvent(int button, int action){
    Global::input.mouseButtonEvent(button, action);
}

void Core::scrollEvent(double distance){
    Global::input.scrollEvent(distance);
}

void Core::framebufferResizeEvent(int width, int height){
    m_app->framebufferResizeEvent(width, height);
}

void Core::windowResizeEvent(int width, int height){
    m_app->windowResizeEvent(width, height);
}
