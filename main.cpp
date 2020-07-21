//
// Created by chris on 7/18/2020.
//

#include "Window.h"
#include "Renderer.h"
#include "Cube.h"
#include "Container.h"
#include "Modifier.h"

#include <vector>
#include <chrono>


struct TransformStorage
{
    explicit TransformStorage(size_t SlabSize) : SlabIndex(-1), SlabSize(SlabSize)
    {
        AllocSlab();
    }
    ~TransformStorage()
    {
        for (size_t idx = 0; idx < Slabs.size(); ++idx)
        {
            delete[] Slabs[idx];
        }
    }

    mat4* Alloc()
    {
        if (SlabIndex >= SlabSize)
        {
            AllocSlab();
        }

        mat4* slab = Slabs.back();
        return slab + SlabIndex++;
    }

    void AllocSlab()
    {
        mat4* slab = new mat4[SlabSize];
        Slabs.push_back(slab);

        SlabIndex = 0;
    }

    std::vector<mat4*> Slabs;
    size_t SlabIndex;
    size_t SlabSize;
};

constexpr size_t TransformSlabSize = 16384 / sizeof(mat4);
static TransformStorage LocalTransforms(TransformSlabSize);
static TransformStorage WorldTransforms(TransformSlabSize);

template<class EntityType, typename ... Args>
EntityType* MakeEntity(std::vector<Entity *>& trackingArray, Args&&... args)
{
    EntityType* result = new EntityType(WorldTransforms.Alloc(), LocalTransforms.Alloc(), std::forward<Args>(args)...);
    trackingArray.push_back(result);
    return result;
}

static v4 Colors[] = {
{ 1, 0, 0, 1 },
{ 0, 1, 0, 1 },
{ 0, 0, 1, 1 },
{ 1, 1, 0, 1 },
{ 1, 0, 1, 1 },
{ 0, 1, 1, 1 },
{ 1, 1, 1, 1 }
};

Container* BuildLevel(int depth, int count, float size, mat4 const& position, std::vector<Entity*>& container)
{
    if (depth < 0)
    {
        return nullptr;
    }

    Container* parent = MakeEntity<Container>(container);

    Modifier* rotator = MakeEntity<Modifier>(container, parent, 0.125f * (float) depth * 2);
    *rotator->mLocalTransform = Mat4Multiply(position, Mat4Translation({ 0, (float) size * -3.5f, 0 }));
    parent->addChild(rotator);

    // We're going to make 2 rows of children spaced by size/2 on either side
    v3 start = { count * -size * 2.5f + size * 1.25f, 0, -size * 1.5f };
    v3 deltax = { size * 2.5f, 0, 0 };
    v3 deltaz = { 0, 0, size * 2.5f };

    v4 color = Colors[depth];

    for (int cubeIdx = 0; cubeIdx < count * 2; ++cubeIdx)
    {
        mat4 leftPosition = Mat4Translation(start + deltax * (float) cubeIdx);
        mat4 rightPosition = Mat4Translation(start + deltax * (float) cubeIdx + deltaz);

        Cube* leftCube = MakeEntity<Cube>(container, size, color);
        Cube* rightCube = MakeEntity<Cube>(container, size, color);

        *leftCube->mLocalTransform = leftPosition;
        *rightCube->mLocalTransform = rightPosition;

        parent->addChild(leftCube);
        parent->addChild(rightCube);

        Container* leftContainer = BuildLevel(depth - 1, count, size, leftPosition, container);
        Container* rightContainer = BuildLevel(depth - 1, count, size, rightPosition, container);

        if (leftContainer != nullptr)
        {
            parent->addChild(leftContainer);
            parent->addChild(rightContainer);
        }
    }

    return parent;
}

using TimePoint = std::chrono::high_resolution_clock::time_point;
struct TimedBlock
{
    explicit TimedBlock(char const* label) : label(label), start(std::chrono::high_resolution_clock::now()) {}
    ~TimedBlock()
    {
        TimePoint now = std::chrono::high_resolution_clock::now();
        printf("[Time Point] %s: %lldus\n", label, std::chrono::duration_cast<std::chrono::microseconds>(now - start).count());
    }

    char const* label;
    TimePoint start;
};

int main(int argc, char *argv[])
{
    std::vector<Entity *> entities;

    Window window;
    Renderer renderer;

    int width = 1024;
    int height = 768;

    window.init("Cool Test", width, height);
    renderer.init(width, height);

    Entity* root = BuildLevel(3, 7, 25, IdentityMat4(), entities);

    while (window.beginFrame())
    {
        {
            TimedBlock update("Update");
            root->update();
        }
        {
            TimedBlock transform("Transform");
            root->updateTransform();
        }
        {
            TimedBlock draw("Draw");
            root->draw(renderer);
        }
        {
            TimedBlock renderFlush("Render Flush");
            renderer.flush();
        }
        {
            TimedBlock swap("Frame Swap");
            window.endFrame();
        }
    }

    for (auto& entity : entities)
    {
        delete entity;
    }
    entities.clear();

    renderer.term();
    window.term();

    return 0;
}