#ifndef DRAWCOMPONENT_H
#define DRAWCOMPONENT_H

#include "Engine/component.h"
#include "Engine/transformcomponent.h"
#include "Graphics/material.h"
#include "Graphics/shape.h"
#include <functional>


class DrawComponent : public Component
{
public:
    DrawComponent(std::weak_ptr<GameObject> parent, std::string shapeString, std::string materialString,
                  std::function<void(void)> drawFunc);
    DrawComponent(std::weak_ptr<GameObject> parent, std::string shapeString, std::string materialString);
    ~DrawComponent();
    void draw();
    void defaultDraw();
private:
    std::shared_ptr<Shape> m_shape;
    std::shared_ptr<Material> m_material;
    std::function<void(void)> m_drawFunc;
    std::shared_ptr<TransformComponent> m_transform;
};

#endif // DRAWCOMPONENT_H
