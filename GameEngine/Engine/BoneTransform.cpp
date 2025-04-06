#include "stdafx.h"
#include "BoneTransform.h"
#include "engineMath.h"

Matrix4 BoneTransform::ToMatrix() const
{
	Matrix4 rotationMat = Matrix4::CreateFromQuaternion(mRot);
	Matrix4 translationMat = Matrix4::CreateTranslation(mPos);
	Matrix4 toMat = rotationMat * translationMat;
	return toMat;
}

BoneTransform BoneTransform::Interpolate(const BoneTransform& a, const BoneTransform& b, float f)
{
	BoneTransform final;
	final.mPos = Vector3::Lerp(a.mPos, b.mPos, f);
	final.mRot = Quaternion::Slerp(a.mRot, b.mRot, f);
	return final;
}
