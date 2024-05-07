#ifndef INPUT_H
#define INPUT_H


#include "glm/vec2.hpp"
#include <map>
class Input {
public:
    void keyEvent(int key, int action);
    void mousePosEvent(double xpos, double ypos);
    void mouseButtonEvent(int button, int action);
    void scrollEvent(double distance);
    bool actionJustPressed(int action);
    bool actionHeld(int action);
    glm::vec2 getMousePos();
    glm::vec2 getMouseDrag();
    float getScrollDist();

    void update(float delta);
private:
    std::map<int, bool> m_inputMap;
    std::map<int, bool> m_justPressedInputMap;
    glm::vec2 m_mousePos;
    glm::vec2 m_mouseDrag;
    float m_scrollDist;
};

#endif // INPUT_H
