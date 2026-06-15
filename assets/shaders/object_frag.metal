#include <metal_stdlib>
#include <metal_types.h>
using namespace metal;

struct VertexOut {
    float4 position [[position]];
    float4 color;
    float2 uv;
};

fragment float4 fragment_main(VertexOut in [[stage_in]], texture2d<float> tex [[texture(0)]], sampler smp [[sampler(0)]]) {
    return tex.sample(smp, in.uv);
    //return in.color;
}