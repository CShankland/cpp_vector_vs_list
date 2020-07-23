#pragma once

//
// Created by chris on 7/18/2020.
//

#include "Entity.h"
#include "Container.h"

class Modifier : public Entity
{
public:
    Modifier(mat4* worldTransform, mat4* localTransform, Entity* target, float speed);

    void update() override;
private:
    Entity* target;
    mat4 mAdjustment;
};
