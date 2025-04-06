#include "stdafx.h"
#include "PointLight.h"
#include "game.h"
#include "jsonUtil.h"
#include "RenderObj.h"

PointLight::PointLight(Game* game, RenderObj* pObj) : Component(pObj) 
{
	mGame = game;
	mPointLightData = mGame->AllocateLight();

}

PointLight::~PointLight()
{
	mGame->FreeLight(mPointLightData);
}

void PointLight::LoadProperties(const rapidjson::Value& properties)
{
	float innerRadius = 0.0f;
	GetFloatFromJSON(properties, "innerRadius", innerRadius);
	float outerRadius = 0.0f;
	GetFloatFromJSON(properties, "outerRadius", outerRadius);
	Vector3 lightColor = Vector3::Zero;
	GetVectorFromJSON(properties, "lightColor", lightColor);
	Vector3 position = Vector3::Zero;
	GetVectorFromJSON(properties, "position", position);

	mPointLightData->lightColor = lightColor;
	mPointLightData->innerRadius = innerRadius;
	mPointLightData->outerRadius = outerRadius;
	mPointLightData->position = position;
}

void PointLight::Update(float deltaTime)
{
	Matrix4 worldMatrix = mRenderObj->GetObjectConstants().c_modelToWorld;
	Vector3 position = worldMatrix.GetTranslation();
	mPointLightData->position = position;
}
