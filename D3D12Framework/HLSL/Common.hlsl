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

struct VS_TEXTURED_INPUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct VS_TEXTURED_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Scene

#define MAX_LIGHTS			16 
#define MAX_MATERIALS		512 

#define POINT_LIGHT			1
#define SPOT_LIGHT			2
#define DIRECTIONAL_LIGHT	3

#define _WITH_LOCAL_VIEWER_HIGHLIGHTING
#define _WITH_THETA_PHI_CONES
//#define _WITH_REFLECT

struct LIGHT
{
    float4 m_cAmbient;
    float4 m_cDiffuse;
    float4 m_cSpecular;
    float3 m_vPosition;
    float m_fFalloff;
    float3 m_vDirection;
    float m_fTheta; //cos(m_fTheta)
    float3 m_vAttenuation;
    float m_fPhi; //cos(m_fPhi)
    bool m_bEnable;
    int m_nType;
    float m_fRange;
    float padding;
};

struct CAMERA
{
    matrix gmtxView;
    matrix gmtxProjection;
    float3 gvCameraPosition;
};

cbuffer cbCamera : register(b0)
{
    CAMERA gCameraData;
}

cbuffer cbLights : register(b1)
{
    LIGHT gLights[MAX_LIGHTS];
    float4 gcGlobalAmbientLight;
    int gnLights;
};

TextureCube gtxtSkyboxCubeMap: register(t0);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pass

StructuredBuffer<matrix> gsbInstanceDatas : register(t1);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Object

struct MATERIAL
{
    float4 xmf4Ambient;
    float4 xmf4Diffuse;
    float4 xmf4Specular; //(r,g,b,a=power)
    float4 xmf4Emissive;

    float fGlossiness;
    float fSmoothness;
    float fSpecularHighlight;
    float fMetallic;
    float fGlossyReflection;
};

cbuffer cbGameObjectInfo : register(b2)
{
    MATERIAL gMaterial;
    int gnInstanceBase;
};

cbuffer cbWorldTransformData : register(b3)
{
    matrix mtxWorld;
}

Texture2D gtxtDiffuseTexture : register(t2);
Texture2D gtxtNormalTexture : register(t3);

SamplerState gSamplerState : register(s0);

#endif