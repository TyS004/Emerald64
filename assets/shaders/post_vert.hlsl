struct VSInput
{
    float3 position : TEXCOORD0;
    float4 color : TEXCOORD1;
    float2 uv : TEXCOORD2;
    float3 norm : TEXCOORD3;
};

struct VSOutput
{
    float4 position : SV_Position;
    float4 color : TEXCOORD0;
    float2 uv : TEXCOORD1;
    float4 norm : TEXCOORD2;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.position = float4(input.position, 1.0f);
    output.color = input.color;
    output.uv = input.uv;
    output.norm = float4(input.norm, 1.0f);
    return output;
}