cbuffer Uniform : register(b0, space1)
{
    float4x4 mvp;
};

struct VertexIn
{
    float3 position : TEXCOORD0;
    float4 color    : TEXCOORD1;
    float2 uv       : TEXCOORD2;
};

struct VertexOut
{
    float4 position : SV_Position;
    float4 color    : TEXCOORD0;
    float2 uv       : TEXCOORD1;
};

VertexOut main(VertexIn input)
{
    VertexOut output;
    output.position = mul(mvp, float4(input.position, 1.0f));
    output.color    = input.color;
    output.uv       = input.uv;
    return output;
}