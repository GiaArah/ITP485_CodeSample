#pragma once
#include "engineMath.h"

namespace Lights
{
	const int maxPointLights = 8;

	struct PointLightData
	{
		Vector3 lightColor;
		float padding1;
		Vector3 position;
		float innerRadius;
		float outerRadius;
		bool isEnabled;
		bool bytePadding[2] = {};
		Vector2 padding2;
	};

	struct LightingConstants
	{
		Vector3 c_ambient;
		float padding;
		PointLightData c_pointLight[maxPointLights];
	};
}