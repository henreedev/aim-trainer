#ifndef APPLICATION_H
#define APPLICATION_H



#include "Graphics/global.h"
#include "Engine/screen.h"

class Application
{
protected:
    std::vector<std::pair<std::shared_ptr<Screen>, bool>> m_screenMap; // Ordered map of all screens and their activity status
public:
    Application() {}
    virtual ~Application(){}
    virtual void update(double deltaTime){
        for (const auto& pair : m_screenMap) {
            if (!pair.second) continue;
            std::shared_ptr<Screen> screenPtr = pair.first;
            screenPtr->update(deltaTime);
        }
    }
    virtual void draw(){
        for (const auto& pair : m_screenMap) {
            if (!pair.second) continue;
            std::shared_ptr<Screen> screenPtr = pair.first;
            screenPtr->draw();
        }
    }
    virtual void keyEvent(int key, int action){
        for (const auto& pair : m_screenMap) {
            if (!pair.second) continue;
            std::shared_ptr<Screen> screenPtr = pair.first;
            screenPtr->keyEvent(key, action);
        }
    }
    virtual void mousePosEvent(double xpos, double ypos){
        for (const auto& pair : m_screenMap) {
            if (!pair.second) continue;
            std::shared_ptr<Screen> screenPtr = pair.first;
            screenPtr->mousePosEvent(xpos, ypos);
        }
    }
    virtual void mouseButtonEvent(int button, int action){
        for (const auto& pair : m_screenMap) {
            if (!pair.second) continue;
            std::shared_ptr<Screen> screenPtr = pair.first;
            screenPtr->mouseButtonEvent(button, action);
        }
    }
    virtual void scrollEvent(double distance){
        for (const auto& pair : m_screenMap) {
            if (!pair.second) continue;
            std::shared_ptr<Screen> screenPtr = pair.first;
            screenPtr->scrollEvent(distance);
        }
    }
    virtual void windowResizeEvent(int width, int height){
        Global::graphics.setWindowSize(glm::ivec2(width, height));
        for (const auto& pair : m_screenMap) {
            if (!pair.second) continue;
            std::shared_ptr<Screen> screenPtr = pair.first;
            screenPtr->windowResizeEvent(width, height);
        }
    }
    virtual void framebufferResizeEvent(int width, int height){
        Global::graphics.setFramebufferSize(glm::ivec2(width, height));
        for (const auto& pair : m_screenMap) {
            if (!pair.second) continue;
            std::shared_ptr<Screen> screenPtr = pair.first;
            screenPtr->framebufferResizeEvent(width, height);
        }
    }
    void switchScreen(std::shared_ptr<Screen> screenPtr, bool isActive) {
        for (auto& pair : m_screenMap) {
            if (pair.first == screenPtr) {
                pair.second = isActive;
                return;
            }
        }
        std::cerr << "Error: Screen pointer not found in the map." << std::endl;
    }

    // Add screens in the order they should be drawn (last screen will be drawn on top)
    void addScreen(std::shared_ptr<Screen> screenPtr, bool isActive) {
        m_screenMap.push_back(std::pair(screenPtr, isActive));
    }

};

#endif // APPLICATION_H
