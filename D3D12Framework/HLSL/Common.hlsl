#ifndef _COMMON_
#define _COMMON_

struct VS_DIFFUSED_INPUT
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VS_DIFFUSED_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

struct MaterialData
{
    float4 xmf4Ambient;
    float4 xmf4Diffuse;
    float4 xmf4Specular; //(r,g,b,a=power)
    float4 xmf4Emissive;

    float fGlossiness = 0.0f;
    float fSmoothness = 0.0f;
    float fSpecularHighlight = 0.0f;
    float fMetallic = 0.0f;
    float fGlossyReflection = 0.0f;
};

struct PerSceneData
{
};

struct PerObjectData
{
};

struct PerInstanceData
{
    matrix mtxWorld;
};

cbuffer cbCameraData : register(b0)
{
    matrix gmtxViewProjection;
};

cbuffer cbWorldTransformData : register(b1)
{
    PerInstanceData instanceData;
}

StructuredBuffer<PerInstanceData> gsbInstanceDatas : register(t0);

#endif