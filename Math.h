#pragma once

//
// Created by chris on 7/18/2020.
//

#include <cmath>
#include <cstdint>

constexpr float PI = 3.14159265358979323846f;
constexpr float DegToRad = PI / 180.0f;

#define MB(number) (number * 1024 * 1024)

inline float
SquareRoot(float value)
{
    return sqrtf(value);
}

inline float
Cosine(float value)
{
    return cosf(value);
}

inline float
Sine(float value)
{
    return sinf(value);
}

union v2
{
    struct
    {
        float x, y;
    };
    float E[2];
};

inline v2
V2(float x, float y)
{
    v2 Result;

    Result.x = x;
    Result.y = y;

    return Result;
}

inline v2
Subtract2(v2 a, v2 b)
{
    return { a.x - b.x, a.y - b.y };
}

inline float
Inner2(v2 a, v2 b)
{
    return a.x * b.x + a.y * b.y;
}

inline float
Distance2Sq(v2 a, v2 b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;

    return dx * dx + dy * dy;
}

union v2i
{
    struct
    {
        int32_t x, y;
    };
    int32_t E[2];
};

inline v2i
V2I(int32_t x, int32_t y)
{
    v2i Result;

    Result.x = x;
    Result.y = y;

    return Result;
}

union v3
{
    struct
    {
        float x, y, z;
    };

    float E[3];
};

inline
v3 V3(float x, float y, float z)
{
    v3 Result = {};

    Result.x = x;
    Result.y = y;
    Result.z = z;

    return Result;
}

inline
v3 Subtract(v3 a, v3 b)
{
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

inline
v3 operator- (v3 a, v3 b)
{
    return Subtract(a, b);
}

inline
v3 Add(v3 a, v3 b)
{
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

inline
v3 operator+ (v3 a, v3 b)
{
    return Add(a, b);
}

inline
v3 Multiply(v3 v, float s)
{
    return { v.x * s, v.y * s, v.z * s };
}

inline
v3 operator* (v3 a, float b)
{
    return Multiply(a, b);
}

inline
float LengthSq(v3 v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline
float Length(v3 v)
{
    return SquareRoot(LengthSq(v));
}

inline
v3 Normalize(v3 v)
{
    float Scale = 1.0f / Length(v);

    return v * Scale;
}

inline
v3 Cross(v3 a, v3 b)
{
    v3 Result = {};

    Result.x = a.y * b.z - a.z * b.y;
    Result.y = a.z * b.x - a.x * b.z;
    Result.z = a.x * b.y - a.y * b.x;

    return Result;
}

inline
float Inner(v3 a, v3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

union v4
{
    struct
    {
        float x, y, z, w;
    };

    float E[4];
};

inline
v4 V4(float x, float y, float z, float w)
{
    return { x, y, z, w };
}

union mat4
{
    float E[16];
    struct
    {
        v4 r0;
        v4 r1;
        v4 r2;
        v4 r3;
    };
};

inline mat4
IdentityMat4()
{
    return {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
    };
}

inline mat4
Mat4Translation(v3 translation)
{
    mat4 result = IdentityMat4();

    result.r0.w = translation.x;
    result.r1.w = translation.y;
    result.r2.w = translation.z;

    return result;
}

inline mat4
Mat4Multiply(mat4 a, mat4 b)
{
    float m00 = a.E[ 0] * b.E[ 0] + a.E[ 1] * b.E[ 4] + a.E[ 2] * b.E[ 8] + a.E[ 3] * b.E[12];
    float m01 = a.E[ 0] * b.E[ 1] + a.E[ 1] * b.E[ 5] + a.E[ 2] * b.E[ 9] + a.E[ 3] * b.E[13];
    float m02 = a.E[ 0] * b.E[ 2] + a.E[ 1] * b.E[ 6] + a.E[ 2] * b.E[10] + a.E[ 3] * b.E[14];
    float m03 = a.E[ 0] * b.E[ 3] + a.E[ 1] * b.E[ 7] + a.E[ 2] * b.E[11] + a.E[ 3] * b.E[15];

    float m10 = a.E[ 4] * b.E[ 0] + a.E[ 5] * b.E[ 4] + a.E[ 6] * b.E[ 8] + a.E[ 7] * b.E[12];
    float m11 = a.E[ 4] * b.E[ 1] + a.E[ 5] * b.E[ 5] + a.E[ 6] * b.E[ 9] + a.E[ 7] * b.E[13];
    float m12 = a.E[ 4] * b.E[ 2] + a.E[ 5] * b.E[ 6] + a.E[ 6] * b.E[10] + a.E[ 7] * b.E[14];
    float m13 = a.E[ 4] * b.E[ 3] + a.E[ 5] * b.E[ 7] + a.E[ 6] * b.E[11] + a.E[ 7] * b.E[15];

    float m20 = a.E[ 8] * b.E[ 0] + a.E[ 9] * b.E[ 4] + a.E[10] * b.E[ 8] + a.E[11] * b.E[12];
    float m21 = a.E[ 8] * b.E[ 1] + a.E[ 9] * b.E[ 5] + a.E[10] * b.E[ 9] + a.E[11] * b.E[13];
    float m22 = a.E[ 8] * b.E[ 2] + a.E[ 9] * b.E[ 6] + a.E[10] * b.E[10] + a.E[11] * b.E[14];
    float m23 = a.E[ 8] * b.E[ 3] + a.E[ 9] * b.E[ 7] + a.E[10] * b.E[11] + a.E[11] * b.E[15];

    float m30 = a.E[12] * b.E[ 0] + a.E[13] * b.E[ 4] + a.E[14] * b.E[ 8] + a.E[15] * b.E[12];
    float m31 = a.E[12] * b.E[ 1] + a.E[13] * b.E[ 5] + a.E[14] * b.E[ 9] + a.E[15] * b.E[13];
    float m32 = a.E[12] * b.E[ 2] + a.E[13] * b.E[ 6] + a.E[14] * b.E[10] + a.E[15] * b.E[14];
    float m33 = a.E[12] * b.E[ 3] + a.E[13] * b.E[ 7] + a.E[14] * b.E[11] + a.E[15] * b.E[15];

    return {
    m00, m01, m02, m03,
    m10, m11, m12, m13,
    m20, m21, m22, m23,
    m30, m31, m32, m33
    };
}

static mat4
CameraLookAt(v3 CameraPosition, v3 CameraTarget, v3 WorldUp)
{
    v3 zAxis = Normalize(CameraTarget - CameraPosition);
    v3 xAxis = Normalize(Cross(WorldUp, zAxis));
    v3 yAxis = Cross(zAxis, xAxis);

    float tx = -Inner(xAxis, CameraPosition);
    float ty = -Inner(yAxis, CameraPosition);
    float tz = -Inner(zAxis, CameraPosition);

    return {
    xAxis.x,  xAxis.y,  xAxis.z,  tx,
    yAxis.x,  yAxis.y,  yAxis.z,  ty,
    -zAxis.x, -zAxis.y, -zAxis.z, -tz,
    0,        0,        0, 1
    };
}

static mat4
ProjectionOrtho(float Right, float Left, float Top, float Bottom, float Near, float Far)
{
    return {
    2.0f / (Right - Left),                     0,                    0, -(Right + Left) / (Right - Left),
    0, 2.0f / (Top - Bottom),                    0, -(Top + Bottom) / (Top - Bottom),
    0,                     0, -2.0f / (Far - Near),     -(Far + Near) / (Far - Near),
    0,                     0,                    0,                                1
    };
}

static mat4
ProjectionPerspective(float Aspect, float FieldOfView, float Near, float Far)
{
    float f = 1.0f / tanf(FieldOfView * 0.5f);

    return {
    f,          0,                           0,                                  0,
    0, f * Aspect,                           0,                                  0,
    0,          0, (Far + Near) / (Near - Far), (2.0f * Near * Far) / (Near - Far),
    0,          0,                          -1,                                  0
    };
}
