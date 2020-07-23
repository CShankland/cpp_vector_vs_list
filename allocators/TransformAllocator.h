#pragma once

//
// Created by chris on 7/22/2020.
//

#include "../Math.h"

class TransformAllocator
{
public:
    virtual mat4* alloc() = 0;
};
