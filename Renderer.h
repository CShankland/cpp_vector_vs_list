#pragma once

//
// Created by chris on 7/18/2020.
//

#include "Math.h"

#include <vector>

class Renderer
{
public:
    void init(int screenWidth, int screenHeight);
    void term();

    void pushCube(mat4 const& transform, v4 const& color, float size);
    void flush();
private:
#pragma pack(push, 1)
    struct CubeRecord
    {
        mat4 position;
        v4 color;
        float size;
    };
#pragma pack(pop)

    std::vector<CubeRecord> mCubes;

    mat4 mProjection;
    mat4 mView;
    float mCameraAngle;

    uint32_t mShader;
    uint32_t mVertexArray;
    uint32_t mIndexBuffer;
    uint32_t mVertexBuffer;
    uint32_t mInstanceDataVertexBuffer;
    size_t instanceBufferSize;
    uint32_t mUniformProjection;
    uint32_t mUniformView;
};
