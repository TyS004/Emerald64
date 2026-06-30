// SDL_GPU D3D12 register convention:
// pixel textures/buffers -> space2, pixel uniform buffers -> space3

// Sampled texture + its sampler (t0/s0 correspond to each other)
Texture2D tex          : register(t0, space2);
SamplerState smp       : register(s0, space2);

struct PointLight
{
    float3 position;
    float4 color;
    float intensity;
};

cbuffer PointLightBuffer : register(b0, space3)
{
    PointLight p_lights[1000];
};

cbuffer NumPLightsBuffer : register(b1, space3)
{
    uint num_p_lights;
};

struct PSInput
{
    float4 position   : SV_Position;
    float4 model_pos  : TEXCOORD0;
    float4 screen_pos : TEXCOORD1;
    float4 color      : TEXCOORD2;
    float2 uv         : TEXCOORD3;
    float4 norm       : TEXCOORD4;
};

static float4 selected_rgba = { 0.0, 0.0, 0.0, 1.0 };
static bool selected = false;

float4 main(PSInput input) : SV_Target
{
    float4 texture_rgba = tex.Sample(smp, input.uv);
    float falloff = 0.5f;
    float3 lit_rgba = { 0.0, 0.0, 0.0};
    
    if (num_p_lights == 0)
    {
        return texture_rgba;
    }
    
    for (int i = 0; i < num_p_lights; ++i)
    {
        float3 dist_vec = p_lights[i].position.xyz - input.model_pos.xyz;
        float dist = max(length(dist_vec), falloff);
        float attenuation = 1.0f / (dist * dist);
        float3 light_dot = max(dot(normalize(dist_vec), normalize(input.norm.xyz)), falloff);
        lit_rgba += texture_rgba.rgb * light_dot * (p_lights[i].intensity * 100) * p_lights[i].color.rgb * attenuation;
    }
    
    return float4(lit_rgba, texture_rgba.a);
}