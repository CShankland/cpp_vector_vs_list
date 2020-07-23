#pragma once

//
// Created by chris on 7/18/2020.
//

#include "Entity.h"

class Container : public Entity
{
public:
    Container(mat4* worldTransform, mat4* localTransform)
        : Entity(worldTransform, localTransform) {}

    void addChild(Entity* entity);

    void update() override;
    void updateTransform() override;
    void draw(Renderer& renderer) override;
private:
    std::vector<Entity*> mChildren;
};
