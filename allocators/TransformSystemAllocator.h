#pragma once

//
// Created by chris on 7/22/2020.
//

#include "TransformAllocator.h"

#include <vector>
#include <memory>

class TransformSystemAllocator : public TransformAllocator
{
public:
    mat4* alloc() override;

private:
    std::vector<std::unique_ptr<mat4>> transforms;
};
