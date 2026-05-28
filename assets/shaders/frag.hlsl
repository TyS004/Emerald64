Texture2D    tex : register(t0, space2);
SamplerState smp : register(s0, space2);

struct VertexOut
{
    float4 position : SV_Position;
    float4 color    : TEXCOORD0;
    float2 uv       : TEXCOORD1;
};

float4 main(VertexOut input) : SV_Target
{
    return tex.Sample(smp, input.uv);
}