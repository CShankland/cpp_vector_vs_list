#pragma once

//
// Created by chris on 7/18/2020.
//

#include "Entity.h"
#include "Math.h"

class Cube : public Entity
{
public:
    Cube(float size, v4 const& color) : mSize(size), mColor(color) {}

    void draw(Renderer& renderer) override;

private:
    v4 mColor;
    float mSize;
};
