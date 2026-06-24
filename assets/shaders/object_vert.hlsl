// SDL_GPU D3D12 register convention:
// vertex textures/buffers -> space0, vertex uniform buffers -> space1

cbuffer ModelBuffer : register(b0, space1)
{
    float4x4 model;
};

cbuffer ViewBuffer : register(b1, space1)
{
    float4x4 view;
};

cbuffer ProjBuffer : register(b2, space1)
{
    float4x4 proj;
};

cbuffer NormalMatBuffer : register(b3, space1)
{
    float4x4 normal_mat;
};

struct VSInput
{
    float3 position : TEXCOORD0;
    float4 color    : TEXCOORD1;
    float2 uv       : TEXCOORD2;
    float3 norm     : TEXCOORD3;
};

struct VSOutput
{
    float4 position   : SV_Position;
    float4 model_pos  : TEXCOORD0;
    float4 screen_pos : TEXCOORD1;
    float4 color      : TEXCOORD2;
    float2 uv         : TEXCOORD3;
    float4 norm       : TEXCOORD4;
};

VSOutput main(VSInput input)
{
    VSOutput output;

    float4 world_pos = mul(model, float4(input.position, 1.0));

    output.position   = mul(proj, mul(view, world_pos));
    output.model_pos  = world_pos;
    output.screen_pos = mul(proj, mul(view, world_pos));
    output.color      = input.color;
    output.uv         = input.uv;
    output.norm       = mul(normal_mat, float4(input.norm, 1.0));

    return output;
}
