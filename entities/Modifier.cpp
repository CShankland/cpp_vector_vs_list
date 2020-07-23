//
// Created by chris on 7/18/2020.
//

#include "Modifier.h"

Modifier::Modifier(mat4 *worldTransform, mat4 *localTransform, Entity *target, float speed)
    : Entity(worldTransform, localTransform), target(target)
{
    float cos = cosf(speed * DegToRad);
    float sin = sinf(speed * DegToRad);

    mAdjustment = {
          cos, 0, sin, 0,
            0, 1,   0, 0,
         -sin, 0, cos, 0,
            0, 0,   0, 1
    };

    target->mLocalTransform = mLocalTransform;
}

void Modifier::update()
{
    *mLocalTransform = Mat4Multiply(*mLocalTransform, mAdjustment);
}
