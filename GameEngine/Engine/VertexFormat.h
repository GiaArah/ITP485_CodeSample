#pragma once
#include "engineMath.h"
#include "Graphics.h"

struct VertexPosColor
{
    Vector3 pos;
    Graphics::Color4 color;
};

struct VertexPosColorUV
{
    Vector3 pos;
    Graphics::Color4 color;
    Vector2 uv;
};

struct VertexPosColorUvNormal
{
    Vector3 pos;
    Vector3 normal;
    Graphics::Color4 color;
    Vector2 uv;
};

struct VertexPosUvNormal
{
    Vector3 pos;
    Vector3 normal;
    Vector2 uv;
};

struct SkinnedInputLayout
{
    Vector3 pos;
    Vector3 normal;
    bool bones[4];
    bool weights[4];
    Vector2 uv;
};

