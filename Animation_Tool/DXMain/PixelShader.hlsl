#include "PackGbuffer.hlsli"

struct Material {
	float3 normal;
	float4 diffuseColor;
	float specExp;
	float specIntensity;
};

cbuffer gMaterialInfo : register(b3) {
	float4 gMaterialColor : packoffset(c0);
	float gSpecExp : packoffset(c1.x);
	float gSpecIntensity : packoffset(c1.y);
}


//texture
Texture2D    gtxtDefault : register(t0);
Texture2D    gtxtSpec : register(t1);
Texture2D    gtxtCP : register(t2);

SamplerState gssDefault : register(s0);


//texture

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

// (보간된) 색 데이터에 대한 통과 함수입니다.
PS_GBUFFER_OUT main(PixelShaderInput input)
{
	PS_GBUFFER_OUT output = (PS_GBUFFER_OUT)0;
	float4 cCPColor = gtxtCP.Sample(gssDefault, input.uv);
	if (cCPColor.x == 0 && cCPColor.y == 0 && cCPColor.z == 0) discard;

	float4 cColor = gtxtDefault.Sample(gssDefault, input.uv) * gMaterialColor;
	float4 cSpecColor = gtxtSpec.Sample(gssDefault, input.uv);
	//cColor *= cSpecColor;
	//cColor = gMaterialColor;

	float Depth = input.position.z/ input.position.w;

	return (PackGBuffer(cColor.xyz, normalize(input.normal), input.positionW, cSpecColor.xyz, gSpecExp, Depth));
	
}
//float4 main(PixelShaderInput input) : SV_TARGET
//{
//
//return gMaterialColor;
//
//}
