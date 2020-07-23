//
// Created by chris on 7/22/2020.
//

#include "TransformSlabAllocator.h"

// Allocate transform memory in 64k chunks
constexpr size_t kSlabSize = 65536 / sizeof(mat4);

TransformSlabAllocator::TransformSlabAllocator()
    : slabIndex(-1), slabSize(kSlabSize)
{
    allocSlab();
}

TransformSlabAllocator::~TransformSlabAllocator()
{
    for (auto& slab : slabs)
    {
        delete[] slab;
    }
}

mat4* TransformSlabAllocator::alloc()
{
    if (slabIndex >= slabSize)
    {
        allocSlab();
    }

    mat4* slab = slabs.back();
    return slab + slabIndex++;
}

void TransformSlabAllocator::allocSlab()
{
    mat4* slab = new mat4[slabSize];
    slabs.push_back(slab);

    slabIndex = 0;
}
