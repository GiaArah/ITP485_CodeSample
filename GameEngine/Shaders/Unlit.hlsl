#include "Constants.hlsl"

struct VIn
{
    float3 position : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct VOut
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 normal : NORMAL0;
    float4 worldPosition : TEXCOORD1;
};

VOut VS(VIn vIn)
{
    VOut output;
    
    // transform input position from model to world space
    float4 toWorldSpace = mul(float4(vIn.position, 1.0), c_modelToWorld);
    output.worldPosition = toWorldSpace;

    // transform position from world to projection space
    output.position = mul(toWorldSpace, c_viewProj);
    
    // transform the normal from model to world space
    output.normal = float4(mul(vIn.normal, (float3x3) c_modelToWorld), 1.0f);
    // normalize vector to ensure unit length
    output.normal = normalize(output.normal);
    
    output.uv = vIn.uv;
    return output;
}

float4 PS(VOut pIn) : SV_TARGET
{
    float4 textureColor = DiffuseTexture.Sample(DefaultSampler, pIn.uv);
    
    // re-normalize normal in pixel shader
    pIn.normal = normalize(pIn.normal);
    
    return float4(textureColor.rgb, 1.0f);     
}
