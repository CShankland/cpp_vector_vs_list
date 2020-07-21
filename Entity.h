#pragma once

//
// Created by chris on 7/18/2020.
//

#include "Math.h"
#include "Renderer.h"

class Entity {
public:
    Entity();
    virtual ~Entity() = default;

    virtual void update();
    virtual void updateTransform();
    virtual void draw(Renderer& renderer);

    Entity* mParent;
    mat4 mWorldTransform;
    mat4 mLocalTransform;
};

