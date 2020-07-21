#pragma once

//
// Created by chris on 7/18/2020.
//

#include "Entity.h"

class Container : public Entity
{
public:
    void addChild(Entity* entity);

    void update() override;
    void updateTransform() override;
    void draw(Renderer& renderer) override;
private:
    std::vector<Entity*> mChildren;
};
