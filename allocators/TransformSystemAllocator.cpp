//
// Created by chris on 7/22/2020.
//

#include "TransformSystemAllocator.h"

mat4* TransformSystemAllocator::alloc()
{
    transforms.emplace_back(std::make_unique<mat4>());
    return transforms.back().get();
}
