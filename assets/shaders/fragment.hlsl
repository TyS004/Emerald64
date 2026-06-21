// SDL_GPU D3D12 register convention:
// pixel textures/buffers -> space2, pixel uniform buffers -> space3

// Sampled texture + its sampler (t0/s0 correspond to each other)
Texture2D tex          : register(t0, space2);
SamplerState smp       : register(s0, space2);

//struct PointLight
//{
//    float3 position;
//    float4 color;
//    float intensity;
//};

// Storage buffer equivalent of `constant PointLight* p_lights`.
// Comes after sampled textures in the t-register order, so t1.
//StructuredBuffer<PointLight> p_lights : register(t1, space2);

// Kept as separate cbuffers, matching the original Metal buffer(1)/buffer(2) split
//cbuffer NumPLightsBuffer : register(b0, space3)
//{
//    uint num_p_lights;
//};

//cbuffer MousePosBuffer : register(b1, space3)
//{
//    float2 mouse_pos;
//};

struct PSInput
{
    float4 position : SV_Position;
    float4 model_pos : TEXCOORD0;
    float4 color     : TEXCOORD1;
    float2 uv        : TEXCOORD2;
    float4 norm      : TEXCOORD3;
};

float4 main(PSInput input) : SV_Target
{
    // Lighting params are still bound/available above, just unused here.
    return tex.Sample(smp, input.uv);
    //return float4(0.0, 1.0, 0.0f, 1.0f);
}
