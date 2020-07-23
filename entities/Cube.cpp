//
// Created by chris on 7/18/2020.
//

#include "Cube.h"

void Cube::draw(Renderer& renderer)
{
    Entity::draw(renderer);

    renderer.pushCube(*mWorldTransform, mColor, mSize);
}
