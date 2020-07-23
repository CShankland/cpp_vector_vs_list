//
// Created by chris on 7/18/2020.
//

#include "Window.h"
#include "Renderer.h"
#include "entities/Cube.h"
#include "entities/Container.h"
#include "entities/Modifier.h"
#include "allocators/TransformAllocator.h"
#include "allocators/TransformSystemAllocator.h"
#include "allocators/TransformSlabAllocator.h"

#include <vector>
#include <chrono>
#include <memory>
#include <unordered_map>

struct EntityContext
{
    template <class Allocator>
    void init()
    {
        localTransformAllocator = std::make_unique<Allocator>();
        worldTransformAllocator = std::make_unique<Allocator>();
    }

    std::vector<Entity *> allEntities;
    std::unique_ptr<TransformAllocator> localTransformAllocator;
    std::unique_ptr<TransformAllocator> worldTransformAllocator;

    template<class EntityType, typename ... Args>
    EntityType* MakeEntity(Args&&... args)
    {
        mat4* worldTransform = worldTransformAllocator->alloc();
        mat4* localTransform = localTransformAllocator->alloc();

        EntityType* result = new EntityType(worldTransform, localTransform, std::forward<Args>(args)...);
        allEntities.push_back(result);
        return result;
    }
};

static v4 Colors[] = {
{ 1, 0, 0, 1 },
{ 0, 1, 0, 1 },
{ 0, 0, 1, 1 },
{ 1, 1, 0, 1 },
{ 1, 0, 1, 1 },
{ 0, 1, 1, 1 },
{ 1, 1, 1, 1 }
};

Container* BuildLevel(int depth, int count, float size, mat4 const& position, EntityContext& ctx)
{
    if (depth < 0)
    {
        return nullptr;
    }

    Container* parent = ctx.MakeEntity<Container>();

    Modifier* rotator = ctx.MakeEntity<Modifier>(parent, (float) (depth * depth) * 0.125f);
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

        Cube* leftCube = ctx.MakeEntity<Cube>(size, color);
        Cube* rightCube = ctx.MakeEntity<Cube>(size, color);

        *leftCube->mLocalTransform = leftPosition;
        *rightCube->mLocalTransform = rightPosition;

        parent->addChild(leftCube);
        parent->addChild(rightCube);

        Container* leftContainer = BuildLevel(depth - 1, count, size, leftPosition, ctx);
        Container* rightContainer = BuildLevel(depth - 1, count, size, rightPosition, ctx);

        if (leftContainer != nullptr)
        {
            parent->addChild(leftContainer);
            parent->addChild(rightContainer);
        }
    }

    return parent;
}

std::unordered_map<char const*, uint64_t> gTimePoints;

using TimePoint = std::chrono::high_resolution_clock::time_point;
struct TimedBlock
{
    explicit TimedBlock(char const* label) : label(label), start(std::chrono::high_resolution_clock::now()) {}
    ~TimedBlock()
    {
        TimePoint now = std::chrono::high_resolution_clock::now();
        gTimePoints[label] += std::chrono::duration_cast<std::chrono::microseconds>(now - start).count();
    }

    char const* label;
    TimePoint start;
};

#define SLAB_ALLOC 0

int main(int argc, char *argv[])
{
    Window window;
    Renderer renderer;

    int width = 1024;
    int height = 768;

    window.init("Cool Test", width, height);
    renderer.init(width, height);

    EntityContext ctx;
#if SLAB_ALLOC
    ctx.init<TransformSlabAllocator>();
#else
    ctx.init<TransformSystemAllocator>();
#endif

    Entity* root = BuildLevel(3, 6, 25, IdentityMat4(), ctx);

    int frameCount = 0;

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

        ++frameCount;
    }

#if SLAB_ALLOC
    printf("=== Slab Allocated Transforms ===\n");
#else
    printf("=== System Allocated Transforms ===\n\n");
#endif

    printf("Total frames: %d\n", frameCount);
    printf("Total cubes: %d\n\n", ctx.allEntities.size());
    for (auto& entry : gTimePoints)
    {
        char const* label = entry.first;
        uint64_t avg = entry.second / frameCount;

        printf("[Time Point] %s: %lldus\n", label, avg);
    }

    renderer.term();
    window.term();

    return 0;
}