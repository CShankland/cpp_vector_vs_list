//
// Created by chris on 7/18/2020.
//

#include "Renderer.h"
#include <GL/glew.h>
#include <cstdio>

#if 1
static uint16_t const kIndexData[] = {
    0, 3, 1,  0, 2, 3,
    2, 7, 3,  2, 6, 7,
    3, 5, 1,  3, 7, 5,
    1, 4, 0,  1, 5, 4,
    0, 6, 2,  0, 4, 2,
    6, 5, 7,  6, 4, 5
};

static v4 const kVertexData[] = {
    { -0.5f,  0.5f, -0.5f, 1 },
    {  0.5f,  0.5f, -0.5f, 1 },
    { -0.5f,  0.5f,  0.5f, 1 },
    {  0.5f,  0.5f,  0.5f, 1 },
    { -0.5f, -0.5f, -0.5f, 1 },
    {  0.5f, -0.5f, -0.5f, 1 },
    { -0.5f, -0.5f,  0.5f, 1 },
    {  0.5f, -0.5f,  0.5f, 1 }
};
#else
static uint16_t const kIndexData[] = {
    0, 1, 2,  0, 2, 3
};

static v4 const kVertexData[] = {
    { -0.5f, -0.5f, 0, 1 },
    { -0.5f,  0.5f, 0, 1 },
    {  0.5f,  0.5f, 0, 1 },
    {  0.5f, -0.5f, 0, 1 }
};
#endif

char kVertexShader[] = R"glsl(
#version 410

uniform mat4 projection;
uniform mat4 view;

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec4 v_color;
layout(location = 2) in float v_size;
layout(location = 3) in mat4 v_transform;

layout(location = 0) out vec4 color;

void main(void) {
    vec4 position = vec4(v_position * v_size, 1.0);
    gl_Position = projection * view * v_transform * position;

    color = v_color;
}
)glsl";

char kFragmentShader[] = R"glsl(
#version 410

layout(location = 0) in vec4 color;
out vec4 frag_color;

void main(void) {
    frag_color = color;
}
)glsl";

void Renderer::init(int screenWidth, int screenHeight)
{
    int vertexShaderLength = sizeof(kVertexShader) / sizeof(char);
    char* vertexShaderSource = &kVertexShader[0];
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, &vertexShaderLength);
    glCompileShader(vertexShader);

    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        char errorBuffer[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, errorBuffer);
        printf("Failed to compile vertex shader: \n%s\n", errorBuffer);
    }

    int fragmentShaderLength = sizeof(kFragmentShader) / sizeof(char);
    char* fragmentShaderSource = &kFragmentShader[0];
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, &fragmentShaderLength);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        char errorBuffer[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, errorBuffer);
        printf("Failed to compile fragment shader: \n%s\n", errorBuffer);
    }

    mShader = glCreateProgram();
    glAttachShader(mShader, vertexShader);
    glAttachShader(mShader, fragmentShader);
    glLinkProgram(mShader);
    glGetProgramiv(mShader, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        char errorBuffer[512];
        glGetProgramInfoLog(mShader, 512, nullptr, &errorBuffer[0]);

        printf("Failed to link shader: \n%s\n", errorBuffer);
    }

    mUniformProjection = glGetUniformLocation(mShader, "projection");
    mUniformView = glGetUniformLocation(mShader, "view");

    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    glGenBuffers(1, &mIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kIndexData), &kIndexData[0], GL_STATIC_DRAW);

    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kVertexData), &kVertexData[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribDivisor(0, 0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &mInstanceDataVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mInstanceDataVertexBuffer);

    size_t stride = sizeof(CubeRecord);
    size_t positionOffset = offsetof(CubeRecord, position);
    size_t colorOffset = offsetof(CubeRecord, color);
    size_t sizeOffset = offsetof(CubeRecord, size);

    // v_color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*) colorOffset);
    glVertexAttribDivisor(1, 1);

    // v_size
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, (void*) sizeOffset);
    glVertexAttribDivisor(2, 1);

    // v_transform
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);

    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*) positionOffset);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, stride, (void*) (positionOffset + 1 * sizeof(v4)));
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, stride, (void*) (positionOffset + 2 * sizeof(v4)));
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, stride, (void*) (positionOffset + 3 * sizeof(v4)));
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);

    mCameraAngle = 0.0f;
    mProjection = ProjectionPerspective((float) screenWidth / (float) screenHeight, 45.0f * DegToRad, 1.0f, 10000.0f);
    instanceBufferSize = 0;
}

void Renderer::term()
{

}

void Renderer::pushCube(mat4 const& transform, v4 const& color, float size)
{
    mCubes.push_back({ Transpose(transform), color, size });
}

void Renderer::flush()
{
    glBindBuffer(GL_ARRAY_BUFFER, mInstanceDataVertexBuffer);

    size_t byteCount = sizeof(CubeRecord) * mCubes.size();
    if (instanceBufferSize >= byteCount)
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, byteCount, (void*) mCubes.data());
    }
    else
    {
        glBufferData(GL_ARRAY_BUFFER, byteCount, (void*) mCubes.data(), GL_DYNAMIC_DRAW);
        instanceBufferSize = byteCount;
    }

    // mCameraAngle += 1;
    float zoom = 1000.0f;
    float x = cosf(mCameraAngle * DegToRad) * zoom;
    float y = zoom;
    float z = sinf(mCameraAngle * DegToRad) * zoom;

    mView = CameraLookAt({ x, y, z }, { 0, 0, 0 }, { 0, 1, 0 });

    glBindVertexArray(mVertexArray);
    glUseProgram(mShader);

    glUniformMatrix4fv(mUniformProjection, 1, GL_TRUE, (GLfloat *) &mProjection);
    glUniformMatrix4fv(mUniformView, 1, GL_TRUE, (GLfloat *) &mView);

    size_t elementCount = sizeof(kIndexData) / sizeof(kIndexData[0]);
    size_t instanceCount = mCubes.size();
    glDrawElementsInstanced(GL_TRIANGLES, elementCount, GL_UNSIGNED_SHORT, nullptr, instanceCount);

    mCubes.clear();
}
