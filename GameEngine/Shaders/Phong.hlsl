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
    
    // calculate the ambient light
    float3 accumulatedDiffuse = (0.0f, 0.0f, 0.0f);
    float3 accumulatedSpecular = (0.0f, 0.0f, 0.0f);
    
    // iterate through all lights and perform diffuse light calculations
    for (int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        if(c_pointLight[i].isEnabled)
        {
            // distance between light source and pixel
            float distFactor = 0.0f;
            float dist = distance(pIn.worldPosition.xyz, c_pointLight[i].position);
            if(dist >= c_pointLight[i].outerRadius)
            {
                distFactor = 0.0f;
            }
            else if(dist <= c_pointLight[i].innerRadius)
            {
                distFactor = 1.0f;
            }
            else
            {
                distFactor = smoothstep(c_pointLight[i].outerRadius, c_pointLight[i].innerRadius, dist);
            }
            
            // diffuse lighting calculations
            float3 diff_l_hat = normalize(c_pointLight[i].position - pIn.worldPosition.xyz);
            float diffuseFactor = dot(pIn.normal.xyz, diff_l_hat);
            diffuseFactor = max(diffuseFactor, 0.0f);
            float3 diffuseContribution = c_pointLight[i].lightColor * diffuseFactor * c_diffuseColor;
            accumulatedDiffuse += (diffuseContribution * distFactor);
            
            // specular lighting calculations
            float3 l_hat = normalize(c_pointLight[i].position - pIn.worldPosition.xyz);
            float3 r_hat = reflect(-l_hat, pIn.normal.xyz);
            float3 v_hat = normalize(c_cameraPosition - pIn.worldPosition.xyz);
            
            float rdotv = dot(r_hat, v_hat);
            float specFactor = max(rdotv, 0.0f);
            specFactor = pow(specFactor, c_specularPower);
            
            float3 spec = c_pointLight[i].lightColor * specFactor * c_specularColor;
            accumulatedSpecular += (spec * distFactor);
        } 
    }

    float3 finalLightColor = c_ambient + accumulatedDiffuse + accumulatedSpecular;
    float3 finalColor = textureColor.rgb * finalLightColor;
    
    return float4(finalColor, 1.0f);     
}
