#include "App.h"

App::App() : m_gameScreen(std::make_shared<GameScreen>()), m_menuScreen(std::make_shared<MenuScreen>()) {
    addScreen(m_gameScreen, true);
    addScreen(m_menuScreen, false);
}

void App::mouseButtonEvent(int button, int action) {
    Application::mouseButtonEvent(button, action);
    switch(button) {
    case GLFW_MOUSE_BUTTON_1:
        // switch to game if menu active
        if(m_screenMap[1].second) {
            switchScreen(m_gameScreen, true);
            switchScreen(m_menuScreen, false);
        }
        break;
    }
}

void App::keyEvent(int key, int action) {
    Application::keyEvent(key, action);
    switch(key) {
    case GLFW_KEY_ESCAPE:
        if(action == GLFW_PRESS){
            // toggle both screens
            switchScreen(m_gameScreen, !m_screenMap[0].second);
            switchScreen(m_menuScreen, !m_screenMap[1].second);
        }
        break;
    }
}
