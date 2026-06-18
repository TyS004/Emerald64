#include <metal_stdlib>
#include <metal_types.h>

using namespace metal;

struct PointLight{
    float3 position;
    float4 color;
    float intensity;
};

struct DirectionalLight{
    float3 transform;
};

struct VertexOut {
    float4 position [[position]];
    float4 model_pos;
    float4 color;
    float2 uv;
    float4 norm;
};

fragment float4 fragment_main(
    VertexOut in [[stage_in]],
    texture2d<float> tex [[texture(0)]],
    sampler smp [[sampler(0)]],
    constant PointLight* p_lights     [[buffer(0)]],
    constant uint32_t& num_p_lights   [[buffer(1)]],
    constant float2& mouse_pos        [[buffer(2)]])
{
    float4 base_color = tex.sample(smp, in.uv);
    if(num_p_lights == 0){
        return base_color;
    }

    float4 p_light_final = {0, 0, 0, 0};
    float falloff = 0.5f;
    for(int i = 0; i < num_p_lights; ++i){
        float3 light_dir = p_lights[i].position - float3(in.model_pos.xyz);
        float dist = max(length(light_dir), falloff);
        float attenuation = 1.0f / (dist * dist);
        float dot_product = max(dot(normalize(light_dir), normalize(in.norm.xyz)), falloff);
        p_light_final += base_color * dot_product * p_lights[i].color * (p_lights[i].intensity * 100) * attenuation;
    }

    return p_light_final;
}
