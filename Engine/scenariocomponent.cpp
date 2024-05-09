#include "scenariocomponent.h"
#include "Engine/gameworld.h"
#include "Warmup/gamescreen.h"
#include "Engine/randomgen.h"

void ScenarioComponent::end() {
    if (score > highScore) {
        highScore = score;
    }
    finished = true;
    // Send score to gamescreen
    GameWorld::gameWorldInstance->removeGameObject(m_parent.lock());
}

void ScenarioComponent::makeGrid() {
    while (GameScreen::dummiesAlive < 1) {
        int x = Random::generateRandomInt(-1, 1);
        int y = Random::generateRandomInt(-1, 1);
        auto pos = defaultSpawn + glm::vec3(gridDist * x, gridDist * y, 0);
        if (pos.x == lastPos.x) {
            continue;
        }
        std::cout << "pos.x: " << pos.x << std::endl;
        std::cout << "pos.y: " << pos.y << std::endl;
        std::cout << "pos.z: " << pos.z << std::endl;

        DummyMaker::makeDummy(pos, 100, 1, FLICK_GRID, false, false);
        lastPos = pos;
    }
}

void ScenarioComponent::makeReact() {
    if (GameScreen::dummiesAlive <= 0) {
        glm::vec3 pos(0);
        if (everyOther) {
            pos = defaultSpawn;
            DummyMaker::makeDummy(pos, 100, 1, FLICK_REACT, false, false);
        } else {
            int randX = Random::generateRandomInt(2, 4);
            int randY = Random::generateRandomInt(0, 2);
            int randFlipX = Random::generateRandomInt(0, 1) * 2 - 1;
            int randFlipY = Random::generateRandomInt(0, 1) * 2 - 1;
            float randScale = Random::generateRandomInt(5, 15) / 10.0f;
            pos = defaultSpawn + glm::vec3(randX * randFlipX, randY * randFlipY, 0);
            DummyMaker::makeDummy(pos, 100, randScale, FLICK_REACT, false, false);
        }
        everyOther = !everyOther;
    }
}

void ScenarioComponent::begin() {
    score = 0.0f;
    staticScore = 0.0f;
    scenarioTimer = 60.0f;
    switch(scenario){
    case TRACK_CLOSE:
        break;
    case TRACK_FAR:
        break;
    case TRACK_MANY:
        break;
    case FLICK_GRID:
        makeGrid();
        break;
    case FLICK_REACT:
        makeReact();
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

float ScenarioComponent::staticScore = 0.0f;

void ScenarioComponent::update(float deltaTime){
    scenarioTimer -= deltaTime;
    if (scenarioTimer <= 0) {
        scenarioTimer = 0;
        end();
    }
    GameScreen::timer = scenarioTimer;

    switch(scenario) {
    case TRACK_CLOSE:
        std::cout << "pog" << std::endl;
        break;
    case TRACK_FAR:
        break;
    case TRACK_MANY:
        break;
    case FLICK_GRID:
        makeGrid();
        break;
    case FLICK_REACT:
        makeReact();
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
