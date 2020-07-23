#pragma once

//
// Created by chris on 7/22/2020.
//

#include "TransformAllocator.h"

#include <vector>

class TransformSlabAllocator : public TransformAllocator
{
public:
    TransformSlabAllocator();
    ~TransformSlabAllocator();

    mat4* alloc() override;

private:
    void allocSlab();

    std::vector<mat4*> slabs;
    size_t slabIndex;
    size_t slabSize;
};

