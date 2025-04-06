#include "stdafx.h"
#include "RenderCube.h"
#include "VertexFormat.h"
#include "VertexBuffer.h"
#include "Graphics.h"
#include "mesh.h"

RenderCube::RenderCube(Material* material) : RenderObj(nullptr)
{
	// Create array for cube vertices
    static VertexPosColorUvNormal cubeVertices[] =
    {
        // first face of cube: -z (red)
        { Vector3(-0.5f, 0.5f, -0.5f), Vector3(0.0f, 0.0f, -1.0f), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f)},
        { Vector3(0.5f, 0.5f, -0.5f), Vector3(0.0f, 0.0f, -1.0f), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f)},
        { Vector3(0.5f, -0.5f, -0.5f), Vector3(0.0f, 0.0f, -1.0f), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f)},
        { Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.0f, 0.0f, -1.0f), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f)},

        // second face of cube: +z (green)
        { Vector3(-0.5f, 0.5f, 0.5f), Vector3(0.0f, 0.0f, 1.0f),  Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f)},
        { Vector3(0.5f, 0.5f, 0.5f), Vector3(0.0f, 0.0f, 1.0f),  Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f)},
        { Vector3(0.5f, -0.5f, 0.5f), Vector3(0.0f, 0.0f, 1.0f), Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f)},
        { Vector3(-0.5f, -0.5f, 0.5f), Vector3(0.0f, 0.0f, 1.0f), Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f)},

        // third face of cube: +x (yellow)
        { Vector3(0.5f, 0.5f, -0.5f), Vector3(1.0f, 0.0f, 0.0f), Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f)},
        { Vector3(0.5f, 0.5f, 0.5f), Vector3(1.0f, 0.0f, 0.0f), Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f)},
        { Vector3(0.5f, -0.5f, 0.5f), Vector3(1.0f, 0.0f, 0.0f), Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f)},
        { Vector3(0.5f, -0.5f, -0.5f), Vector3(1.0f, 0.0f, 0.0f), Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f)},

        // fourth face of cube: -x (blue)
        { Vector3(-0.5f, 0.5f, 0.5f), Vector3(-1.0f, 0.0f, 0.0f), Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f)},
        { Vector3(-0.5f, 0.5f, -0.5f), Vector3(-1.0f, 0.0f, 0.0f), Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f)},
        { Vector3(-0.5f, -0.5f, -0.5f), Vector3(-1.0f, 0.0f, 0.0f), Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f)},
        { Vector3(-0.5f, -0.5f, 0.5f), Vector3(-1.0f, 0.0f, 0.0f),  Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f)},

        // fifth face of cube: +y (magenta)
        { Vector3(-0.5f, 0.5f, -0.5f), Vector3(0.0f, 1.0f, 0.0f), Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f)},
        { Vector3(0.5f, 0.5f, -0.5f), Vector3(0.0f, 1.0f, 0.0f), Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f)},
        { Vector3(0.5f, 0.5f, 0.5f), Vector3(0.0f, 1.0f, 0.0f), Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f)},
        { Vector3(-0.5f, 0.5f, 0.5f), Vector3(0.0f, 1.0f, 0.0f), Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f)},

        // sixth face of cube: -y (cyan)
        { Vector3(-0.5f, -0.5f, 0.5f), Vector3(0.0f, -1.0f, 0.0f), Graphics::Color4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f)},
        { Vector3(0.5f, -0.5f, 0.5f), Vector3(0.0f, -1.0f, 0.0f), Graphics::Color4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f)},
        { Vector3(0.5f, -0.5f, -0.5f), Vector3(0.0f, -1.0f, 0.0f), Graphics::Color4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f)},
        { Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.0f, -1.0f, 0.0f), Graphics::Color4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f)}
    };

	// Create array for cube indicies
	static uint16_t cubeIndices[] =
	{
		// -z face
        2, 1, 0,
        3, 2, 0,

		// +z face
        7, 4, 5,
        6, 7, 5,

		// +x face
		10, 9, 8,
		11, 10, 8,

		// -x face
        14, 13, 12,
        15, 14, 12,

		// +y face
		16, 17, 18,
		19, 16, 18,

		// -y face
		20, 21, 22,
		23, 20, 22,
	};

	VertexBuffer* mVertexBuffer = new VertexBuffer(cubeVertices, ARRAY_SIZE(cubeVertices), sizeof(cubeVertices[0]), cubeIndices, ARRAY_SIZE(cubeIndices), sizeof(cubeIndices[0]));
    mMesh = new Mesh(mVertexBuffer, material);
}
