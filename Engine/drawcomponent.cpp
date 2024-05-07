#include "drawcomponent.h"
#include "Engine/gameobject.h"
#include "Engine/transformcomponent.h"
#include "Graphics/global.h"

DrawComponent::DrawComponent(std::weak_ptr<GameObject> parent, std::string shapeString, std::string materialString, std::function<void(void)> drawFunc)
{
    m_shape = Global::graphics.getShape(shapeString);
    m_material = Global::graphics.getMaterial(materialString);
    m_drawFunc = drawFunc;
}

DrawComponent::DrawComponent(std::weak_ptr<GameObject> parent, std::string shapeString, std::string materialString)
{
    m_shape = Global::graphics.getShape(shapeString);
    m_material = Global::graphics.getMaterial(materialString);
    if (parent.lock()->hasComponent<TransformComponent>()) {
        m_transform = parent.lock()->getComponent<TransformComponent>();
    }
}

DrawComponent::~DrawComponent(){}

void DrawComponent::draw(){
    if(m_drawFunc) {
        m_drawFunc();
    } else {
        defaultDraw();
    }
}

void DrawComponent::defaultDraw() {
    if (m_transform) {
        Global::graphics.drawShape(m_shape, m_transform->getModelMatrix(), m_material);
    } else {
        // No transform to draw with
    }
}


