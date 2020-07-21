#pragma once

//
// Created by chris on 7/18/2020.
//

#include "Entity.h"
#include "Container.h"

class Modifier : public Entity
{
public:
    Modifier(mat4* worldTransform, mat4* localTransform, Entity* target, float speed)
        : Entity(worldTransform, localTransform), rotation(0), speed(speed), target(target) {}

    void update() override;
private:
    float rotation;
    float speed;
    Entity* target;
};
