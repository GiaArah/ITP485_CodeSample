#pragma once
#include "engineMath.h"

class BoneTransform
{
public:
	Matrix4 ToMatrix() const;

	static BoneTransform Interpolate(const BoneTransform& a, const BoneTransform& b, float f);

	Quaternion mRot;
	Vector3 mPos;
};

