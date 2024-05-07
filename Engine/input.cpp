#include "input.h"
#include <iostream>

void Input::update(float delta) {
    m_justPressedInputMap.clear();
    m_mouseDrag = glm::vec2();
    m_scrollDist = 0.0f;
}

void Input::keyEvent(int key, int action) {
    m_inputMap.insert_or_assign(key, action);
}
void Input::mousePosEvent(double xpos, double ypos) {
    auto newPos = glm::vec2(xpos, ypos);
    m_mouseDrag = newPos - m_mousePos;
    m_mousePos = newPos;
}
void Input::mouseButtonEvent(int button, int action) {
    m_inputMap.insert_or_assign(button, action);
}
void Input::scrollEvent(double distance) {
    m_scrollDist = distance;
}
bool Input::actionJustPressed(int action) {
    return m_justPressedInputMap.contains(action) ? m_justPressedInputMap.at(action) : false;
}
bool Input::actionHeld(int action) {
    return m_inputMap.contains(action) ? m_inputMap.at(action) : false;
}
glm::vec2 Input::getMousePos() {
    return m_mousePos;
}
glm::vec2 Input::getMouseDrag(){
    return m_mouseDrag;
}
float Input::getScrollDist() {
    return m_scrollDist;
}
