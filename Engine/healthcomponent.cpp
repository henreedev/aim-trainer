#include "healthcomponent.h"
#include "gameworld.h"
#include "Warmup/gamescreen.h"
void HealthComponent::die() {
    GameWorld::gameWorldInstance->removeGameObject(m_parent.lock());
    GameScreen::dummiesAlive--;
}
