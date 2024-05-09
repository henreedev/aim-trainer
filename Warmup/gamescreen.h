#ifndef GAMESCREEN_H
#define GAMESCREEN_H


#include "Engine/scenario.h"
#include "Engine/screen.h"
#include <memory>
#include "Graphics/global.h"


class GameWorld;
class TransformComponent;

class GameObject;



class GameScreen : public Screen
{
private:
    std::shared_ptr<GameWorld> m_gameWorld;
    std::shared_ptr<Camera> m_camera;
    float m_cameraOffset;
    std::shared_ptr<Shape> m_cube;
    std::shared_ptr<ModelTransform> m_modelMat;
    std::vector<std::shared_ptr<Light>> m_lights;

    bool m_key_w_held;
    bool m_key_s_held;
    bool m_key_a_held;
    bool m_key_d_held;
    bool m_mouse_2_held;
    glm::vec2 m_mouse_prev_pos;

    glm::vec3 m_pos;
    glm::vec3 m_vel;
    const float GRAVITY = 5.0f;
    const glm::vec3 worldBoundsMin = glm::vec3(-20, -20, -20);
    const glm::vec3 worldBoundsMax = glm::vec3(20, 20, 20);
    bool m_grounded;
    std::shared_ptr<TransformComponent> tc;
    void loadScenario(Scenario scenario);

public:
    static std::shared_ptr<GameObject> ray;
    static Scenario currScen;
    static int dummiesAlive;
    GameScreen();

    void reset();
    void update(double deltaTime) override;
    void draw() override;
    void keyEvent(int key, int action) override;
    void mousePosEvent(double xpos, double ypos) override;
    void mouseButtonEvent(int button, int action) override;
    void scrollEvent(double distance) override;
    void windowResizeEvent(int width, int height) override;
    void framebufferResizeEvent(int width, int height) override;
};

#endif // GAMESCREEN_H
