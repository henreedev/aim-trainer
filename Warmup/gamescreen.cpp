#include "gamescreen.h"
#include "Engine/aisystem.h"
#include "Engine/behaviortree.h"
#include "Engine/gameworld.h"
#include "Engine/gameobject.h"
#include "Engine/cameracomponent.h"
#include "Engine/charactercontrollercomponent.h"
#include "Engine/charactercontrollersystem.h"
#include "Engine/drawcomponent.h"
#include "Engine/meshloader.h"
#include "Engine/ellipsecollider.h"
#include "GLFW/glfw3.h"


using namespace std;

GameScreen::GameScreen()
{
    Global::graphics.addMaterial("grass", "Resources/Images/grass.png", 0.3);
    Global::graphics.addMaterial("tile", "Resources/Images/tile.png", 0.9);
    m_lights.push_back(make_shared<Light>(LightType::DIRECTIONAL, glm::normalize(glm::vec3(1, -1, 0)), glm::vec3(1, 0, 1)));
    Global::graphics.setLights(m_lights);

    reset();

}

void GameScreen::update(double deltaTime){
    m_gameWorld->update(deltaTime);
}

void GameScreen::draw(){
    Global::graphics.setClearColor(glm::vec3(0.0f, 0.5f, 0.8f));
    Global::graphics.clearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Global::graphics.bindShader("phong");
    Global::graphics.setGlobalData(glm::vec3(0.5f));
    m_gameWorld->draw();
    if (tc->getPos().y > 7){
        Global::graphics.bindShader("text");
        Global::graphics.drawUIText(Global::graphics.getFont("opensans"), "You reached the top!",
                                    glm::ivec2(Global::graphics.getFramebufferSize().x / 2, Global::graphics.getFramebufferSize().y / 2),
                                    AnchorPoint::TopCenter,
                                    Global::graphics.getFramebufferSize().x,
                                    0.8f, 0.1f, glm::vec3(.8, .8, 1));
    }
}

void GameScreen::reset() {
    // MAKE PLAYER GAMEOBJECT
    shared_ptr<GameObject> player = make_shared<GameObject>();
    weak_ptr<GameObject> player_weak = player;
    tc = make_shared<TransformComponent>(player_weak, glm::vec3(0, 3, 0), 2.0);
    player->addComponent<TransformComponent>(tc);
    player->addComponent<DrawComponent>(make_shared<DrawComponent>(player_weak, "sphere", "grass"));
    shared_ptr<CameraComponent> cc = make_shared<CameraComponent>(player_weak, 0.015, true, true);
    player->addComponent<CameraComponent>(cc);

    std::shared_ptr<EllipseCollider> collider = std::make_shared<EllipseCollider>(tc);
    player->addComponent<CharacterControllerComponent>(make_shared<CharacterControllerComponent>(player_weak, 5.0, 7.0));

    shared_ptr<CollisionComponent> coc = make_shared<CollisionComponent>(player_weak, collider, false);
    player->addComponent<CollisionComponent>(coc);

    // MAKE FLOOR GAMEOBJECT
    shared_ptr<GameObject> floor = make_shared<GameObject>();
    weak_ptr<GameObject> floor_weak = floor;
    shared_ptr<DrawComponent> floorDC = make_shared<DrawComponent>(floor_weak, "quad", "tile", [](){
        auto quad = Global::graphics.getShape("quad");
        shared_ptr<ModelTransform> quadModelMat = make_shared<ModelTransform>();
        int floorDim = 20;
        float halfDim = floorDim / 2.0f;
        for(int i = 0; i < floorDim; i++) {
            for(int j = 0; j < floorDim; j++) {
                quadModelMat->setPos(glm::vec3(1.0f * i - halfDim, -0.5f, 1.0f * j - halfDim));
                Global::graphics.drawShape(quad, quadModelMat, Global::graphics.getMaterial("grass"));
            }
        }
    });

    floor->addComponent<DrawComponent>(floorDC);

    // add obstacle
    shared_ptr<GameObject> obstacle = make_shared<GameObject>();
    weak_ptr<GameObject> obstacle_weak = obstacle;
    shared_ptr<ModelTransform> mesh_transform = make_shared<ModelTransform>();
    mesh_transform->setPos(glm::vec3(0, 0.5f, 0));
    mesh_transform->setScale(glm::vec3(1.0, 1.0, 1.0));
    shared_ptr<TransformComponent> o_tc = make_shared<TransformComponent>(obstacle_weak, mesh_transform);
    obstacle->addComponent(o_tc);
    shared_ptr<TriMesh> trimesh = MeshLoader::getMeshFromVertices(Global::graphics.addShape("test", "Resources/Meshes/environment3.obj"), mesh_transform);
    MeshLoader::addNavmeshToTrimesh(Global::graphics.addShape("test_navmesh", "Resources/Meshes/environment3nav.obj"), mesh_transform, trimesh);
    AiSystem::navMesh = trimesh->navmesh;
    obstacle->addComponent(make_shared<DrawComponent>(obstacle_weak, "test", "tile"));

    shared_ptr<CollisionComponent> o_coc = make_shared<CollisionComponent>(obstacle_weak, trimesh);
    obstacle->addComponent(o_coc);
    // add BALL
    shared_ptr<GameObject> ball = make_shared<GameObject>();
    weak_ptr<GameObject> ball_weak = ball;
    shared_ptr<ModelTransform> ball_transform = make_shared<ModelTransform>();
    ball_transform->setPos(glm::vec3(10.0f, 10.0f, 0.0f));
    ball_transform->setScale(glm::vec3(2.0, 2.0, 2.0));
    shared_ptr<TransformComponent> ball_tc = make_shared<TransformComponent>(ball_weak, ball_transform);
    ball->addComponent<TransformComponent>(ball_tc);
    ball->addComponent<DrawComponent>(make_shared<DrawComponent>(ball_weak, "sphere", "tile"));

    shared_ptr<CharacterControllerComponent> ball_ccc = make_shared<CharacterControllerComponent>(ball_weak);
    ball->addComponent<CharacterControllerComponent>(ball_ccc);
    std::shared_ptr<EllipseCollider> ball_collider = std::make_shared<EllipseCollider>(ball_tc);

    shared_ptr<CollisionComponent> ball_coc = make_shared<CollisionComponent>(ball_weak, ball_collider, false);
    ball->addComponent<CollisionComponent>(ball_coc);

    // add ENEMY
    shared_ptr<GameObject> enemy = make_shared<GameObject>();
    weak_ptr<GameObject> enemy_weak = enemy;
    shared_ptr<ModelTransform> enemy_transform = make_shared<ModelTransform>();
    enemy_transform->setPos(glm::vec3(5.0f, 5.0f, 0.0f));
    enemy_transform->setScale(glm::vec3(2.0, 2.0, 2.0));
    shared_ptr<TransformComponent> enemy_tc = make_shared<TransformComponent>(enemy_weak, enemy_transform);
    enemy->addComponent<TransformComponent>(enemy_tc);
    enemy->addComponent<DrawComponent>(make_shared<DrawComponent>(enemy_weak, "sphere", "tile"));

    shared_ptr<CharacterControllerComponent> enemy_ccc = make_shared<CharacterControllerComponent>(enemy_weak);
    enemy->addComponent<CharacterControllerComponent>(enemy_ccc);
    std::shared_ptr<EllipseCollider> enemy_collider = std::make_shared<EllipseCollider>(enemy_tc);

    shared_ptr<CollisionComponent> enemy_coc = make_shared<CollisionComponent>(enemy_weak, enemy_collider, false);
    enemy->addComponent<CollisionComponent>(enemy_coc);

    AiSystem::player = player;
    // MAKE AI BEHAVIOR TREE

    // Root: Selector
    // Left child: Sequence of Condition (path exists?), Action (Follow path)
    // Right child: Action (jump)
    BTNode* pathExists = new Condition([&enemy_ccc](float deltaTime) {
        if (!enemy_ccc->m_path.empty() && !glm::all(glm::equal(enemy_ccc->target, glm::vec2(0.0f)))) {
            return BTStatus::SUCCESS;
        } else {
            return BTStatus::FAILURE;
        }
    });

    BTNode* jump = new Action([&enemy_ccc](float deltaTime) {
//        enemy_ccc->jump();
        return BTStatus::SUCCESS;
    });

    Selector* root = new Selector(pathExists, jump);
    shared_ptr<AiComponent> enemy_ai = make_shared<AiComponent>(enemy_weak, root);
    enemy->addComponent<AiComponent>(enemy_ai);

    // ADD PARTS TO WORLD
    m_gameWorld = make_shared<GameWorld>();
    m_gameWorld->addSystem<DrawSystem>(make_shared<DrawSystem>());
    m_gameWorld->addSystem<CharacterControllerSystem>(make_shared<CharacterControllerSystem>(5.0));
    m_gameWorld->addSystem<CameraSystem>(make_shared<CameraSystem>());
    m_gameWorld->addSystem<CollisionSystem>(make_shared<CollisionSystem>());
//    m_gameWorld->addSystem<AiSystem>(make_shared<AiSystem>());
    m_gameWorld->addGameObject(player);
    m_gameWorld->addGameObject(obstacle);
    m_gameWorld->addGameObject(ball);
//    m_gameWorld->addGameObject(enemy);
    m_gameWorld->addGameObject(floor);

}

void GameScreen::keyEvent(int key, int action){

}

void GameScreen::mousePosEvent(double xpos, double ypos){

}

void GameScreen::mouseButtonEvent(int button, int action){


}

void GameScreen::scrollEvent(double distance){
}

void GameScreen::framebufferResizeEvent(int width, int height){
}

void GameScreen::windowResizeEvent(int width, int height){
    // TODO: Find gameobject with camera component, tell it to resize to width and height.
}
