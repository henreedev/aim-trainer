#include "healthcomponent.h"
#include "gameworld.h"
#include "Warmup/gamescreen.h"
void HealthComponent::die() {
    canDie = false;
    auto parent = m_parent.lock();
    if (parent) {
        GameWorld::gameWorldInstance->removeGameObject(parent);
        GameScreen::dummiesAlive--;
        GameScreen::score++;
        std::cout << "Dummy killed" << std::endl;
    } else {
        std::cerr << "Failed to remove dead dummy: parent is expired." << std::endl;
    }
}
