#include "scenariocomponent.h"
#include "Engine/gameworld.h"
#include "Warmup/gamescreen.h"
void ScenarioComponent::end() {
    if (score > highScore) {
        highScore = score;
    }
    // Send score to gamescreen
    GameWorld::gameWorldInstance->removeGameObject(m_parent.lock());
}

void ScenarioComponent::begin() {
    score = 0.0f;
    scenarioTimer = 60.0f;
    switch(scenario){
    case TRACK_CLOSE:
        std::cout << "pog" << std::endl;
        break;
    case TRACK_FAR:
        break;
    case TRACK_MANY:
        break;
    case FLICK_GRID:
        while (GameScreen::dummiesAlive < 5) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++){

                }
            }
        }
    case FLICK_REACT:
        break;
    case FLICK_BOUNCE:
        break;
    case SWITCH_360:
        break;
    case SWITCH_SEQUENTIAL:
        break;
    case SWITCH_GRAVITY:
        break;
    default:
        break;
    }
}

void ScenarioComponent::update(float deltaTime){
    switch(scenario) {
    case TRACK_CLOSE:
        std::cout << "pog" << std::endl;
        break;
    case TRACK_FAR:
        break;
    case TRACK_MANY:
        break;
    case FLICK_GRID:
        break;
    case FLICK_REACT:
        break;
    case FLICK_BOUNCE:
        break;
    case SWITCH_360:
        break;
    case SWITCH_SEQUENTIAL:
        break;
    case SWITCH_GRAVITY:
        break;
    default:
        break;
    }
}
