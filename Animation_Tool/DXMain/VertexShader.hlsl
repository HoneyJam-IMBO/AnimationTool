cbuffer ViewProjectionConstantBuffer : register(b0)
{
	matrix gmtxViewProjection;
};
cbuffer WorldConstantBuffer : register(b1){
	matrix gmtxWorld;
};
struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	//float4x4 mtxWorld : INSTANCEPOS;
};

// 픽셀 셰이더를 통과한 픽셀당 색 데이터입니다.
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};


VS_OUTPUT main(VS_INPUT input, uint instanceID : SV_InstanceID){
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
	output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);
	//output.position = mul(float4(input.position, 1.0f), gmtxViewProjection);
	output.normal = mul(input.normal, (float3x3)gmtxWorld);

	output.uv = input.uv;
	//output.color = float4(1.0f,1.0f,0.0f, 1.0f);

	return output;
}
