//
// Created by chris on 7/18/2020.
//

#include "Modifier.h"

void Modifier::update()
{
    rotation += speed * DegToRad;

    float cos = cosf(rotation);
    float sin = sinf(rotation);

    mat4 rotationMatrix = {
         cos, 0, sin, 0,
           0, 1,   0, 0,
        -sin, 0, cos, 0,
          0,    0, 0, 1
    };

    target->mLocalTransform = Mat4Multiply(mLocalTransform, rotationMatrix);
}
